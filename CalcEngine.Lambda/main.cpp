#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/platform/Environment.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <aws/lambda-runtime/runtime.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include "../CalcEngine/calc_manager.h"
#include "../CalcEngine/helpers.h"
#include "../CalcEngine/reader_csv.h"
#include "../CalcEngine/io_manager.h"

using namespace aws::lambda_runtime;

bool download_file(
    Aws::S3::S3Client const &client,
    Aws::String const &bucket,
    Aws::String const &key,
    Aws::String const &file_path);

inline bool file_exists(const std::string &name);
std::string encode(Aws::String const &filename, Aws::String &output);
char const TAG[] = "CalcEngine-Lambda";

static invocation_response my_handler(invocation_request const &req, Aws::S3::S3Client const &client, Aws::SQS::SQSClient const &sqs)
{
    try
    {
        //Process SQS Payload
        using namespace Aws::Utils::Json;
        JsonValue jsonPayload(req.payload);

        if (!jsonPayload.WasParseSuccessful())
        {
            AWS_LOGSTREAM_INFO(TAG, "Payload parsing failure! - " << jsonPayload.GetErrorMessage() << "\nPayload: " << req.payload);
            return invocation_response::failure("JSON FAILED", "InvalidJSON");
        }

        //Processing single message only.
        auto payload = jsonPayload.View();
        auto messages = payload.GetArray("Records");
        auto body = messages[0].GetString("body");
        auto message_handle = messages[0].GetString("receiptHandle");

        JsonValue jsonMessage(body);

        if (!jsonMessage.WasParseSuccessful())
        {
            AWS_LOGSTREAM_INFO(TAG, "Message parsing failure! - " << jsonMessage.GetErrorMessage() << "\nPayload: " << body);
            std::cout << jsonMessage.GetErrorMessage() << std::endl;
            return invocation_response::failure("JSON FAILED", "InvalidJSON");
        }

        auto message = jsonMessage.View();

        if (!message.ValueExists("calculationid") || !message.GetObject("calculationid").IsString() ||
            !message.ValueExists("jsonid") || !message.GetObject("jsonid").IsString() ||
            !message.ValueExists("s3jsonbucket") || !message.GetObject("s3jsonbucket").IsString() ||
            !message.ValueExists("s3datatypesbucket") || !message.GetObject("s3datatypesbucket").IsString() ||
            !message.ValueExists("s3valuesbucket") || !message.GetObject("s3valuesbucket").IsString() ||
            !message.ValueExists("s3jsonkey") || !message.GetObject("s3jsonkey").IsString() ||
            !message.ValueExists("s3datatypeskey") || !message.GetObject("s3datatypeskey").IsString() ||
            !message.ValueExists("s3valueskey") || !message.GetObject("s3valueskey").IsString())
            {
                AWS_LOGSTREAM_INFO(TAG, "Message does not contain all expected input values.");
                return invocation_response::failure("Missing input value", "InvalidJSON");
            }

        auto calculation_id = message.GetString("calculationid"); 
        auto json_id = message.GetString("jsonid"); 
        auto json_bucket = message.GetString("s3jsonbucket");
        auto datatypes_bucket = message.GetString("s3datatypesbucket");
        auto values_bucket = message.GetString("s3valuesbucket");
        auto json_key = message.GetString("s3jsonkey");
        auto datatypes_key = message.GetString("s3datatypeskey");
        auto values_key = message.GetString("s3valueskey");

        if (mkdir("/tmp/json", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            if (errno != EEXIST)
            {
                // something else
                AWS_LOGSTREAM_INFO(TAG, "Cannot create json directory.");
                return invocation_response::failure("Failed!", "Cannot create json directory.");                
            }
        }

        AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << json_bucket << "/" << json_key);
        Aws::String json_path = "/tmp/json/" + json_id;
        auto err_json = download_file(client, json_bucket, json_key, json_path);
        if (!err_json)
        {
            return invocation_response::failure("Failed!", "DownloadFailure - JSON");
        }

        AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << datatypes_bucket << "/" << datatypes_key);
        Aws::String datatypes_path = "/tmp/data_" + calculation_id;
        auto err_datatypes = download_file(client, datatypes_bucket, datatypes_key, datatypes_path);
        if (!err_datatypes)
        {
            return invocation_response::failure("Failed!", "DownloadFailure - DataTypes");
        }

        AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << values_bucket << "/" << values_key);
        Aws::String values_path = "/tmp/values_" + calculation_id;
        auto err_values = download_file(client, values_bucket, values_key, values_path);
        if (!err_values)
        {
            return invocation_response::failure("Failed!", "DownloadFailure - Values");
        }

        //PROCESS REQUEST!
        AWS_LOGSTREAM_INFO(TAG, "Loading io manager - datatypes: " << datatypes_path << " - values: " << values_path);
        io_manager iomanager(datatypes_path, values_path);

        AWS_LOGSTREAM_INFO(TAG, "Loading calc manager - json: " << json_path);
        calc_manager manager;
        manager.load_from_json(json_path);

        auto start = std::chrono::steady_clock::now();
        manager.run_calc(iomanager);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

        std::stringstream ss;
        for (auto entity : iomanager._input_order)
        {
            auto entry = iomanager._inputs.at(entity);
            for (auto value : entry)
            {
                ss << value.first << ": " << value.second.value << " ";
            }
            ss << '\n';
        }

        Aws::SQS::Model::DeleteMessageRequest dm_req;
        dm_req.SetQueueUrl("https://sqs.us-west-2.amazonaws.com/542100369852/calc-build-dev");
        dm_req.SetReceiptHandle(message_handle);

        auto dm_out = sqs.DeleteMessage(dm_req);
        if (dm_out.IsSuccess())
        {
            std::cout << "Successfully deleted message from queue" << std::endl;
        }
        else
        {
            std::cout << "Error deleting message from queue" << std::endl;
        }

        return invocation_response::success(ss.str(), "application/json");
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return invocation_response::failure("FAILED!", "Unable to complete request.");
    }
}

std::function<std::shared_ptr<Aws::Utils::Logging::LogSystemInterface>()> GetConsoleLoggerFactory()
{
    return [] {
        return Aws::MakeShared<Aws::Utils::Logging::ConsoleLogSystem>(
            "console_logger", Aws::Utils::Logging::LogLevel::Trace);
    };
}

int main()
{
    using namespace Aws;
    SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
    options.loggingOptions.logger_create_fn = GetConsoleLoggerFactory();

    std::cout << "Creating\n"
              << std::endl;
    InitAPI(options);
    {
        Client::ClientConfiguration config;
        config.region = Aws::Environment::GetEnv("AWS_REGION");
        config.caFile = "/etc/pki/tls/certs/ca-bundle.crt";

        auto credentialsProvider = Aws::MakeShared<Aws::Auth::EnvironmentAWSCredentialsProvider>(TAG);
        S3::S3Client client(credentialsProvider, config);
        SQS::SQSClient sqs(credentialsProvider, config);
        auto handler_fn = [&client, &sqs](aws::lambda_runtime::invocation_request const &req) {
            std::cout << "context: " << req.client_context << std::endl;
            std::cout << "payload: " << req.payload << std::endl;
            return my_handler(req, client, sqs);
        };
        run_handler(handler_fn);
    }
    ShutdownAPI(options);
    return 0;
}

inline bool file_exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool download_file(
    Aws::S3::S3Client const &client,
    Aws::String const &bucket,
    Aws::String const &key,
    Aws::String const &file_path)
{
    using namespace Aws;

    AWS_LOGSTREAM_INFO(TAG, "Attempting Download");
    S3::Model::GetObjectRequest request;
    request.WithBucket(bucket).WithKey(key);

    auto outcome = client.GetObject(request);
    if (outcome.IsSuccess())
    {
        AWS_LOGSTREAM_INFO(TAG, "Download completed!");
        auto &s = outcome.GetResultWithOwnership().GetBody();
        std::ofstream output_file(file_path, std::ios::binary);
        output_file << s.rdbuf();
        return true;
    }
    else
    {
        AWS_LOGSTREAM_ERROR(TAG, "Failed with error: " << outcome.GetError());
        return false;
    }
}
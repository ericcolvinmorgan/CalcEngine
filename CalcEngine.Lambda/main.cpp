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
#include <aws/lambda-runtime/runtime.h>
#include <fstream>
#include <iostream>
#include <memory>
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

std::string encode(Aws::String const &filename, Aws::String &output);
char const TAG[] = "LAMBDA_ALLOC";

static invocation_response my_handler(invocation_request const &req, Aws::S3::S3Client const &client)
{
    using namespace Aws::Utils::Json;
    JsonValue json(req.payload);
    if (!json.WasParseSuccessful())
    {
        return invocation_response::failure("Failed to parse input JSON", "InvalidJSON");
    }

    auto v = json.View();

    if (!v.ValueExists("s3bucket") || !v.GetObject("s3bucket").IsString() ||
        !v.ValueExists("s3jsonkey") || !v.GetObject("s3jsonkey").IsString() ||
        !v.ValueExists("s3datatypeskey") || !v.GetObject("s3datatypeskey").IsString() ||
        !v.ValueExists("s3valueskey") || !v.GetObject("s3valueskey").IsString())
    {
        return invocation_response::failure("Missing input value", "InvalidJSON");
    }

    auto bucket = v.GetString("s3bucket");
    auto json_key = v.GetString("s3jsonkey");
    auto datatypes_key = v.GetString("s3datatypeskey");
    auto values_key = v.GetString("s3valueskey");

    AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << bucket << "/" << json_key);
    Aws::String json_path = "/tmp/file.json";
    auto err_json = download_file(client, bucket, json_key, json_path);
    if (!err_json)
    {
        return invocation_response::failure("Failed!", "DownloadFailure - JSON");
    }

    AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << bucket << "/" << datatypes_key);
    Aws::String datatypes_path = "/tmp/data.csv";
    auto err_datatypes = download_file(client, bucket, datatypes_key, datatypes_path);
    if (!err_datatypes)
    {
        return invocation_response::failure("Failed!", "DownloadFailure - DataTypes");
    }

    AWS_LOGSTREAM_INFO(TAG, "Attempting to download file from s3://" << bucket << "/" << values_key);
    Aws::String values_path = "/tmp/values.csv";
    auto err_values = download_file(client, bucket, values_key, values_path);
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

    return invocation_response::success(ss.str(), "application/json");
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
    InitAPI(options);
    {
        Client::ClientConfiguration config;
        config.region = Aws::Environment::GetEnv("AWS_REGION");
        config.caFile = "/etc/pki/tls/certs/ca-bundle.crt";

        auto credentialsProvider = Aws::MakeShared<Aws::Auth::EnvironmentAWSCredentialsProvider>(TAG);
        S3::S3Client client(credentialsProvider, config);
        auto handler_fn = [&client](aws::lambda_runtime::invocation_request const &req) {
            return my_handler(req, client);
        };
        run_handler(handler_fn);
    }
    ShutdownAPI(options);
    return 0;
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
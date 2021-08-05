#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include "../CalcEngine/calc_manager.h"
#include "../CalcEngine/helpers.h"
#include "../CalcEngine/reader_csv.h"
#include "../CalcEngine/io_manager.h"
#include "../CalcEngine/logging/logger_interface.h"
#include "../CalcEngine/logging/console_log.h"

int main(int argc, char **argv)
{
	Logging::LoggerInterface *logger;

	auto consoleLogger = Logging::ConsoleLogger(Logging::LogLevel::Error);
	logger = &consoleLogger;

	if (argc != 5)
	{
		std::cout << "Please provide the following file location parameters: <calculation definition JSON path> <data types definition CSV path> <calculation values CSV path> <CSV output path>";
	}
	else
	{
		std::stringstream messages;
		std::string jsonPath = argv[1];
		std::string dataTypesPath = argv[2];
		std::string valuesPath = argv[3];
		std::string outputPath = argv[4];
		io_manager iomanager(dataTypesPath, valuesPath);

		calc_manager manager;
		manager.load_from_json(jsonPath);

		auto start = std::chrono::steady_clock::now();
		manager.run_calc(iomanager);
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;

		messages << "elapsed time: " << elapsed_seconds.count() << "s\n";

		int scen = 1;
		std::vector<std::string> output_headers;
		for (auto output = iomanager._output_types.begin(); output != iomanager._output_types.end(); output++)
		{
			output_headers.push_back(output->first);
		}

		std::sort(output_headers.begin(), output_headers.end());

		//Save Output
		std::filebuf buf;
		buf.open(outputPath, std::ios::out);
		std::ostream outputStream(&buf);

		for (int s = 0; s < output_headers.size(); s++)
		{
			outputStream << output_headers[s];
			if (s < output_headers.size() - 1)
				outputStream << ",";
			else
				outputStream << "\n";
		}

		for (auto entity : iomanager._input_order)
		{
			auto entry = iomanager._inputs.at(entity);
			for (int s = 0; s < output_headers.size(); s++)
			{
				outputStream << entry[output_headers[s]].value;
				if (s < output_headers.size() - 1)
					outputStream << ",";
				else
					outputStream << "\n";
			}
		}
		buf.close();

		logger->Log(Logging::LogLevel::Error, messages);
	}
}
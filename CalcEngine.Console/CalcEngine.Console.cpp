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

int main(int argc, char** argv)
{
	Logging::LoggerInterface* logger;

	auto consoleLogger = Logging::ConsoleLogger(Logging::LogLevel::Error);
	logger = &consoleLogger;

	if (argc != 5)
	{
		std::cout << "Please provide the following file location parameters: <calculation definition JSON path> <data types definition CSV path> <calculation values CSV path> <CSV output path>";
	}
	else
	{
		std::stringstream messages;
		messages << "CALCULATION START\n";
		std::string jsonPath = argv[1];
		std::string dataTypesPath = argv[2];
		std::string valuesPath = argv[3];
		io_manager iomanager(dataTypesPath, valuesPath);
				
		calc_manager manager;
		manager.load_from_json(jsonPath);

		auto start = std::chrono::steady_clock::now();
		manager.run_calc(iomanager);
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		
		messages << "elapsed time: " << elapsed_seconds.count() << "s\n";

		int scen = 1;
		for (auto entity : iomanager._input_order)
		{
			auto entry = iomanager._inputs.at(entity);
			for (auto value : entry)
			{
				std::cout << value.first << ": " << value.second.value << " ";
			}
			std::cout << '\n';
		}
		
		logger->Log(Logging::LogLevel::Error, messages);
	}
}
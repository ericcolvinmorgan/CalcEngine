#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>   
#include <fstream>
#include "../CalcEngine/calc_manager.h"
#include "../CalcEngine/helpers.h"
#include "../CalcEngine/reader_csv.h"
#include "../CalcEngine/io_manager.h"

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		std::cout << "Please provide the following file location parameters: calculation definition JSON, data types definition CSV, calculation values CSV";
	}
	else
	{
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
		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

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
	}
}
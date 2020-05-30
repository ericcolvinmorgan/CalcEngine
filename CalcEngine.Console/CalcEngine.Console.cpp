// CalcEngine.Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <ctime>   
#include "../CalcEngine/calcManager.h"
#include "../CalcEngine/helpers.h"

int main()
{
	int items = 10;
	double initialEarnings = 1000;
	std::vector<std::unordered_map<std::string, double>> values(items);
	for (int i = 0; i < items; i++)
	{
		std::unordered_map<std::string, double> entry;
		entry.emplace(std::piecewise_construct,
			std::forward_as_tuple("Calculation-F7"),
			std::forward_as_tuple(initialEarnings));

		entry.emplace(std::piecewise_construct,
			std::forward_as_tuple("Accum E and P and Taxes-D7"),
			std::forward_as_tuple(0));

		entry.emplace(std::piecewise_construct,
			std::forward_as_tuple("Accum E and P and Taxes-D17"),
			std::forward_as_tuple(500));

		values[i] = entry;
	}

	calc_manager manager;
	manager.load_from_json("C:\\Users\\ericm\\Documents\\source\\repos\\CalcEngine\\ExcelParser.Tests\\TestFiles\\TestFile.json");
	
	auto start = std::chrono::steady_clock::now();
	manager.run_calc(values);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

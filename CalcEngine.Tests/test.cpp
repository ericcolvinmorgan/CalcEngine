#include "pch.h"
#include "../CalcEngine/calcManager.h"
#include "../CalcEngine/helpers.h"

TEST(TestCaseName, TestName) {

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
	//EXPECT_TRUE(manager.LoadFromJSON("..\\CalcEngine.Tests\\Basic1.json"));
	EXPECT_TRUE(manager.load_from_json("C:\\Users\\ericm\\Documents\\source\\repos\\CalcEngine\\ExcelParser.Tests\\TestFiles\\TestFile.json"));
	EXPECT_TRUE(manager.run_calc(values));
	//EXPECT_EQ(1, 1);
}


TEST(TestCaseName, column_alpha_from_int) {
	EXPECT_EQ("", column_alpha_from_int(0));
	EXPECT_EQ("", column_alpha_from_int(16385));
	EXPECT_EQ("XFD", column_alpha_from_int(16384));
	EXPECT_EQ("A", column_alpha_from_int(1));
	EXPECT_EQ("CZP", column_alpha_from_int(2720));
	EXPECT_EQ("ZZ", column_alpha_from_int(702));
	EXPECT_EQ("AA", column_alpha_from_int(27));
}
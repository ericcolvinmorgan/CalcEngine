#include "pch.h"
#include "../CalcEngine/calc_manager.h"
#include "../CalcEngine/helpers.h"

TEST(TestCaseName, TestName) {
	io_manager value_manager("..\\CalcEngine.Tests\\EPDataTypes.csv", "..\\CalcEngine.Tests\\EPValues.csv");
	calc_manager manager;
	//EXPECT_TRUE(manager.LoadFromJSON("..\\CalcEngine.Tests\\Basic1.json"));
	EXPECT_TRUE(manager.load_from_json("..\\CalcEngine.Tests\\TestFile.json"));
	EXPECT_TRUE(manager.run_calc(value_manager));
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
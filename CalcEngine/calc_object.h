#pragma once
#include "cell_detail.h"
#include "excel_functions_enum.h"
#include <string>
#include <unordered_map>
#include <vector>

class calc_object
{
public:
	calc_object();
	~calc_object();
	bool load_from_json(std::string file_location);
	bool run_calc();

private:
	std::unordered_map<std::string, cell_detail> _cell_details;
	std::unordered_map<std::string, excel_functions_enum> _excel_functions;
	std::vector<std::string> _processOrder;
};
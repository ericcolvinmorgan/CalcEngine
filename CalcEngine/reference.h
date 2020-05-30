#pragma once
#include <string>
#include <nlohmann/json.hpp>

class reference
{
public:
	reference();
	reference(const nlohmann::json& json);
	~reference();

private:
	std::string _worksheet;
	std::string _reference_string;
	std::string _min_location;
	int _min_row = 0;
	bool _min_row_absolute = false;
	int _min_column = 0;
	bool _min_column_absolute = false;
	std::string _max_location;
	int _max_row = 0;
	bool _max_row_absolute = false;
	int _max_column = 0;
	bool _max_column_absolute = false;
	std::string _named_range;
};
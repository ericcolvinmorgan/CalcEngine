#include "cell_node.h"

calc_tree::cell_node::cell_node()
{
}

calc_tree::cell_node::cell_node(const nlohmann::json& json) : calc_tree::node(json)
{
	if (json["Worksheet"].is_null())
	{
		_worksheet = "";
	}
	else { 
		json.at("Worksheet").get_to(_worksheet);
	}

	if (json["Row"].is_null())
	{
		_row = 0;
	}
	else {
		json.at("Row").get_to(_row);
	}

	if (json["Column"].is_null())
	{
		_column = 0;
	}
	else {
		json.at("Column").get_to(_column);
	}
}

bool calc_tree::cell_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value)
{
	return _process_node(cell_details, excel_functions, value);
}

bool calc_tree::cell_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value)
{
	return _process_node(cell_details, excel_functions, value);
}

cell_detail calc_tree::cell_node::get_cell_detail(std::unordered_map<std::string, cell_detail> cell_details)
{
	std::string ref(_token);
	ref.erase(std::remove(ref.begin(), ref.end(), '$'), ref.end());
	cell_detail cell = cell_details.at(_worksheet + "-" + ref);
	return cell;
}

template<typename T>
inline bool calc_tree::cell_node::_process_node(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values)
{	
	std::string ref(_token);
	ref.erase(std::remove(ref.begin(), ref.end(), '$'), ref.end());
	
	T default_value;
	this->_get_default_value(default_value);
	T value = default_value;
	
	std::unordered_map<std::string, cell_detail>::const_iterator cellItem = cell_details.find(_worksheet + "-" + ref);
	if (cellItem != cell_details.end())
	{
		cell_detail cell = cell_details.at(_worksheet + "-" + ref);
		cell.get_value(value);
	}

	std::vector<T> entry(1, default_value);
	values.push_back(entry);
	values[0][0] = value;

	return true;
}

calc_tree::cell_node::~cell_node()
{
}
#include "number_node.h"

calc_tree::number_node::number_node()
{
}

calc_tree::number_node::number_node(const nlohmann::json &json) : calc_tree::node(json)
{
	_token_value = std::stod(_token);
}

bool calc_tree::number_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>> &values)
{
	(void)cell_details;	   //Currently unused in this node type.
	(void)excel_functions; //Currently unused in this node type.
	std::vector<std::string> entry(1, 0);
	values.push_back(entry);
	values[0][0] = _token;
	return true;
}

bool calc_tree::number_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>> &values)
{
	(void)cell_details;	   //Currently unused in this node type.
	(void)excel_functions; //Currently unused in this node type.
	std::vector<double> entry(1, 0);
	values.push_back(entry);
	values[0][0] = _token_value;
	return true;
}

calc_tree::number_node::~number_node()
{
}

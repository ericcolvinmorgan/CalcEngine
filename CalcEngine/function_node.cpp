#include "function_node.h"


calc_tree::function_node::function_node()
{
}

calc_tree::function_node::function_node(const nlohmann::json& json) : calc_tree::node(json)
{
}

bool calc_tree::function_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value)
{
	//TODO - Implement
	(void)cell_details; //Currently unused in this node type.
	(void)excel_functions; //Currently unused in this node type.
	(void)value; //Currently unused in this node type.
	return false;
}

bool calc_tree::function_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value)
{
	return _ProcessNode(cell_details, excel_functions, value);
}

template<typename T>
inline bool calc_tree::function_node::_ProcessNode(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values)
{
	double calcValues[2] = {0.};
	if (_left_node != nullptr)
	{
		std::vector<std::vector<double>> nodeValue;
		_left_node->get_value(cell_details, excel_functions, nodeValue);
		calcValues[0] = nodeValue[0][0];
	}

	if (_right_node != nullptr)
	{
		std::vector<std::vector<double>> nodeValue;
		_right_node->get_value(cell_details, excel_functions, nodeValue);
		calcValues[1] = nodeValue[0][0];
	}

	//TODO - Implement binary operator logic.

	double returnValue = 0.0;
	if (_token == "+")
	{
		returnValue = calcValues[0] + calcValues[1];
	}
	else if (_token == "-")
	{
		returnValue = calcValues[0] - calcValues[1];
	}
	else if (_token == "*")
	{
		returnValue = calcValues[0] * calcValues[1];
	}
	else if (_token == "/")
	{
		returnValue = calcValues[0] / calcValues[1];
	}
	else if (_token == "^")
	{
		returnValue = pow(calcValues[0], calcValues[1]);
	}

	T defaultValue;
	this->_get_default_value(defaultValue);
	std::vector<T> entry(1, defaultValue);
	values.push_back(entry);
	values[0][0] = returnValue;

	return true;
}

calc_tree::function_node::~function_node()
{
}

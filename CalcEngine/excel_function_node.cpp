#include "excel_function_node.h"

calc_tree::excel_function_node::excel_function_node()
{
}

calc_tree::excel_function_node::excel_function_node(const nlohmann::json& json) : calc_tree::node(json)
{
	if (!json["Arguments"].is_null())
	{
		for (auto argumentNode : json["Arguments"])
		{
			_arguments.push_back(calc_tree::node::create_node(argumentNode));
		}
	}
}

bool calc_tree::excel_function_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value)
{
	(void)cell_details; //Unused in this node type.
	(void)excel_functions; //Unused in this node type.
	(void)value; //Unused in this node type.
	return false;
}

bool calc_tree::excel_function_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value)
{
	return _ProcessNode(cell_details, excel_functions, value);
}

template<typename T>
inline bool calc_tree::excel_function_node::_ProcessNode(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values)
{
	std::vector<double> args;
	for (std::shared_ptr<node> arg : _arguments)
	{
		std::vector<std::vector<double>> argValues;
		arg->get_value(cell_details, excel_functions, argValues);
		
		for (auto rowValue = argValues.begin(); rowValue != argValues.end(); rowValue++)
		{
			for (auto colValue = rowValue->begin(); colValue != rowValue->end(); colValue++)
			{
				args.push_back(*colValue);
			}
		}
	}

	double value = 0.0;
	if (_token == "SUM")
	{
		for (double arg : args)
		{
			value += arg;
		}
	}

	else if (_token == "AVERAGE")
	{
		int count = 0;
		for (double arg : args)
		{
			value += arg;
			count += 1;
		}
		value /= count;
	}

	else if (_token == "COUNT")
	{
		int count = args.size();
		value = count;
	}

	else if (_token == "MIN")
	{
		int min = args[0];
		for (double arg : args)
		{
			if (arg < min)
			{
				min = arg;
			}
		}
		value = min;
	}

	else if (_token == "MAX")
	{
		int max = args[0];
		for (double arg : args)
		{
			if (arg > max)
			{
				max = arg;
			}
		}
		value = max;
	}

	T defaultValue;
	this->_get_default_value(defaultValue);
	std::vector<T> entry(1, defaultValue);
	values.push_back(entry);
	values[0][0] = value;

	return true;
}

calc_tree::excel_function_node::~excel_function_node()
{
}

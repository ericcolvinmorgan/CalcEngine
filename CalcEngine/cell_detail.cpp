#include "cell_detail.h"
#include <stack>

cell_detail::cell_detail()
{
}

cell_detail::cell_detail(const nlohmann::json& json)
{
	json.at("WorksheetCell").get_to(_worksheet_cell);
	json.at("Worksheet").get_to(_worksheet);
	json.at("Cell").get_to(_cell);
	json.at("Row").get_to(_row);
	json.at("Column").get_to(_column);
	if (!json["Formula"].is_null())
	{
		_formula = formula_detail(json["Formula"]);
	}
	json.at("Value").get_to(_value);
	json.at("Type").get_to(_type);
	/*_references = std::vector<Reference>();
	for (auto jsonRef : json["References"])
	{
		_references.emplace_back(jsonRef);
	}*/
	json.at("FormulaTreeValid").get_to(_formula_tree_valid);
	if (!json["FormulaTree"].is_null())
	{
		_formula_tree = calc_tree::node::create_node(json["FormulaTree"]);
	}
}


cell_detail::~cell_detail()
{
}

void cell_detail::calculate(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::unordered_map<std::string, io_value> inputValues)
{
	switch (_type)
	{
	case cell_type::value:
	{
		std::unordered_map<std::string, io_value>::const_iterator inputItem = inputValues.find(_worksheet + "-" + _cell);
		if (inputItem != inputValues.end())
		{
			_calculated_value = inputItem->second.value;
		}
		else
		{
			_calculated_value = std::stod(_value);
		}
		_dirty = false;
	}
		break;

		case cell_type::string:
		//TODO - Handle string logic
		break;

	case cell_type::formula:
		if (_formula_tree != nullptr)
		{
			std::vector<std::vector<double>> values;
			_formula_tree->get_value(cell_details, excel_functions, values);
			_calculated_value = values[0][0];
		}
		break;
	}
}

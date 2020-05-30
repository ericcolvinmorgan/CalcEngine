#include "cell_node.h"
#include "excel_function_node.h"
#include "function_node.h"
#include "node.h"
#include "number_node.h"
#include "reference_node.h"

calc_tree::node::node()
{
}

calc_tree::node::node(const nlohmann::json& json)
{
	json.at("NodeType").get_to(_nodeType);
	json.at("IsValid").get_to(_is_valid);
	json.at("IsCell").get_to(_is_cell);
	json.at("IsConstant").get_to(_is_constant);
	json.at("IsFunction").get_to(_is_function);
	json.at("Token").get_to(_token);
	if (!json["LeftNode"].is_null())
	{
		_left_node = calc_tree::node::create_node(json["LeftNode"]);
	}

	if (!json["RightNode"].is_null())
	{
		_right_node = calc_tree::node::create_node(json["RightNode"]);
	}
}

calc_tree::node::~node()
{
}


std::shared_ptr<calc_tree::node> calc_tree::node::create_node(const nlohmann::json& json)
{
	if (!json["NodeType"].is_null())
	{
		std::string node_type;
		json["NodeType"].get_to(node_type);

		if (node_type == "CellNode")
		{
			return std::make_shared<calc_tree::cell_node>(json);
		}

		if (node_type == "ExcelFunctionNode")
		{
			return std::make_shared<calc_tree::excel_function_node>(json);
		}

		if (node_type == "FunctionNode")
		{
			return std::make_shared<calc_tree::function_node>(json);
		}

		if (node_type == "NumberNode")
		{
			return std::make_shared<calc_tree::number_node>(json);
		}

		if (node_type == "ReferenceNode")
		{
			return std::make_shared<calc_tree::reference_node>(json);
		}
	}
	return NULL;
}

std::vector<std::shared_ptr<calc_tree::node>> calc_tree::node::get_dependency_nodes()
{
	std::vector<std::shared_ptr<node>> dependencies;
	if (_left_node != nullptr)
	{
		dependencies.push_back(_left_node);
	}

	if (_right_node != nullptr)
	{
		dependencies.push_back(_right_node);
	}
	return dependencies;
}
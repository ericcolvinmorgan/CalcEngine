#pragma once
#include "node.h"
namespace calc_tree {
	class number_node :
		public node
	{
	public:
		number_node();
		number_node(const nlohmann::json& json);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value);
		~number_node();
	};
}
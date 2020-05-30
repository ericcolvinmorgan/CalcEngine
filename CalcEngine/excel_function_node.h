#pragma once
#include "node.h"
namespace calc_tree {
	class excel_function_node :
		public node
	{
	public:
		excel_function_node();
		excel_function_node(const nlohmann::json& json);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value);
		~excel_function_node();
	
	private:
		std::vector<std::shared_ptr<node>> _arguments;
		template<typename T>
		bool _ProcessNode(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values);
	};
}
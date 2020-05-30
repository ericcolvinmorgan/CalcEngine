#pragma once
#include "node.h"
namespace calc_tree {
	class reference_node :
		public node
	{
	public:
		reference_node();
		reference_node(const nlohmann::json& json);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value);
		~reference_node();

	private:
		std::vector<reference> _references;
		template<typename T>
		bool _ProcessNode(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values);
	};
}
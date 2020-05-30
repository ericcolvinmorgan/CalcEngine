#pragma once
#include "node.h"
namespace calc_tree {
	class cell_node :
		public node
	{
	public:
		cell_node();
		cell_node(const nlohmann::json& json);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value);
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value);
		cell_detail get_cell_detail(std::unordered_map<std::string, cell_detail> cell_details);
		std::string get_worksheet() { return _worksheet; }
		int get_row() { return _row; }
		int get_column() { return _column; }
		~cell_node();

	private:
		std::string _worksheet;
		int _row;
		int _column;
		template<typename T>
		bool _process_node(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values);
	};
}
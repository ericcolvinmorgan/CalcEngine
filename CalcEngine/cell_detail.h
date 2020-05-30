#pragma once
#include "formula_detail.h"
#include "excel_functions_enum.h"
#include "reference.h"
#include "node.h"
#include "io_manager.h"
#include <memory>
#include <vector>

namespace calc_tree {
	class node;
}
class cell_detail
{
public:
	enum class cell_type { formula, value, string };
	cell_detail();
	cell_detail(const nlohmann::json& json);
	~cell_detail();

	std::string worksheet_cell() { return _worksheet_cell; }
	void calculate(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::unordered_map<std::string, io_value> input_values);
	void get_value(std::string& v) { v = _value; }
	void get_value(double& v) { v = _calculated_value; }
	void set_value(double& v) { _calculated_value = v; }
	int get_row() { return _row; }
	int get_column() { return _column; }
	std::string get_worksheet() { return _worksheet; }

private:
	std::string _worksheet_cell;
	std::string _worksheet;
	std::string _cell;
	int _row;
	int _column;
	formula_detail _formula;
	std::string _value;
	cell_type _type;
	std::vector<reference> _references;
	bool _formula_tree_valid;
	std::shared_ptr<calc_tree::node> _formula_tree = NULL;
	double _calculated_value = 0;
	bool _dirty = true;
};
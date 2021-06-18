#include "reference_node.h"
#include "cell_node.h"
#include "helpers.h"

calc_tree::reference_node::reference_node()
{
	//TODO - Load from References property in JSON
}

calc_tree::reference_node::reference_node(const nlohmann::json& json) : calc_tree::node(json)
{
}

bool calc_tree::reference_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value)
{
	return _ProcessNode(cell_details, excel_functions, value);
}

bool calc_tree::reference_node::get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>>& value)
{
	return _ProcessNode(cell_details, excel_functions, value);
}

template<typename T>
inline bool calc_tree::reference_node::_ProcessNode(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<T>>& values)
{
	(void)excel_functions; //Currently unused in this node type.
	int startRow = 0, startCol = 0, endRow = 0, endCol = 0;
	std::string rangeWorksheet = "";
	if (_left_node != nullptr)
	{

		//Process Left Node]
		if (_left_node->is_cell())
		{
			std::shared_ptr<cell_node> leftNode = std::dynamic_pointer_cast<cell_node>(_left_node);
			rangeWorksheet = leftNode->get_worksheet();
			startRow = leftNode->get_row();
			startCol = leftNode->get_column();
		}
	}

	if (_right_node != nullptr)
	{
		if (_right_node->is_cell())
		{
			std::shared_ptr<cell_node> rightNode = std::dynamic_pointer_cast<cell_node>(_right_node);			
			endRow = rightNode->get_row();
			endCol = rightNode->get_column();
		}
	}

	//Ensure a valid reference exists to allow for processing.
	if (startRow == 0 || startCol == 0 || endRow == 0 || endCol == 0)
	{
		return false;
	}

	//Initialize Vector
	T defaultValue;
	this->_get_default_value(defaultValue);
	
	std::vector<T> cols((endCol - startCol) + 1, defaultValue);
	values = std::vector<std::vector<T>>((endRow - startRow) + 1, cols);

	int row = 0;
	for (int r = startRow - 1; r < endRow; r++)
	{
		int col = 0;
		for (int c = startCol - 1; c < endCol; c++)
		{
			std::string ref = column_alpha_from_int(c + 1) + std::to_string(r + 1);
			std::unordered_map<std::string, cell_detail>::const_iterator mapItem = cell_details.find(rangeWorksheet + "-" + ref);
			T cell_value = defaultValue;
			if (mapItem != cell_details.end())
			{
				cell_detail cell = mapItem->second;
				cell.get_value(cell_value);
			}
			values[row][col++] = cell_value;
		}
		row++;
	}

	return true;
}

calc_tree::reference_node::~reference_node()
{
}
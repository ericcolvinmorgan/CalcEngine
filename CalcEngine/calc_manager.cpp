#include "calc_manager.h"
#include "reference.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stack>

using json = nlohmann::json;

calc_manager::calc_manager()
{
	
}

calc_manager::~calc_manager()
{

}

bool calc_manager::load_from_json(std::string fileLocation)
{
	//auto x = [&count](int depth, json::parse_event_t event, json& parsed) {
	//	if (event == json::parse_event_t::object_end)
	//	{
	//		++count;
	//		return false; // do not store the object value
	//	}
	//	else
	//	{
	//		return true;
	//	}
	//};

	std::ifstream ifs(fileLocation);
	//json::parse(ifs, x);
	json jsonDetail = json::parse(ifs);
	
	for (auto order : jsonDetail["ProcessOrder"])
	{
		_process_order.push_back(order["WorksheetCell"]);
	}

	_cell_details.max_load_factor(.05f);
	for (auto jsonCell : jsonDetail["CellDetails"])
	{
		std::string key;
		jsonCell.at("WorksheetCell").get_to(key);
		_cell_details.emplace(std::piecewise_construct,
			std::forward_as_tuple(key),
			std::forward_as_tuple(jsonCell));
	}

	return true;
}

bool calc_manager::run_calc(io_manager& io_manager)
{	
	for (std::string &entry : io_manager._input_order)
	{
		io_manager.update_entries(entry);
		std::unordered_map<std::string, io_value>& calc_values = io_manager._inputs.at(entry);
		
		//Iterate through cells in the determined calculation order
		for (std::string processItem : _process_order)
		{
			//Get current cell
			cell_detail& cell = _cell_details.at(processItem);
			cell.calculate(_cell_details, _excel_functions, calc_values);
		}

		for (auto s : io_manager._output_types)
		{
			cell_detail& cell = _cell_details.at(s.first);
			double calc_value;
			cell.get_value(calc_value);
			calc_values.emplace(std::piecewise_construct,
				std::forward_as_tuple(s.first),
				std::forward_as_tuple(io_value{ true, calc_value, "" }));
		}
	}
	return true;
}
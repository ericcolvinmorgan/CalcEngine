#include "io_manager.h"
#include "reader_csv.h"
#include <algorithm>
#include <vector>

io_manager::io_manager(std::string data_types_path, std::string values_path)
{
	_build_data_types(data_types_path);
	_build_inputs(values_path);
}

void io_manager::update_entries(std::string entry)
{
	std::unordered_map<std::string, io_value>& calc_values = _inputs.at(entry);

	for (auto input : _input_types)
	{
		if (input.second.type == io_type::type_enum::rollover)
		{
			io_value parent = calc_values[input.second.params[1]];
			if (parent.string_value != "")
			{
				std::unordered_map<std::string, io_value>& parent_values = _inputs.at(parent.string_value);
				calc_values[input.first] = parent_values[input.second.params[0]];
			}
		}
	}
}

void io_manager::_build_data_types(std::string data_types_path)
{
	std::vector<std::string> headers = { "Reference", "Name", "Data Type", "Data Parameters" };
	reader_csv reader(data_types_path, headers);

	for (auto entry : reader)
	{
		std::vector<std::string> params;
		for (int i = 3; i < entry.size(); i++)
		{
			params.push_back(entry[i]);
		}

		io_type::type_enum entry_type = _type_from_string(entry[2]);

		switch (entry_type)
		{
		case io_type::type_enum::value:
		case io_type::type_enum::rollover:
			_input_types.emplace(std::piecewise_construct,
				std::forward_as_tuple(entry[0]),
				std::forward_as_tuple(io_type{ entry[1], entry_type, params }));
			break;

		case io_type::type_enum::output:
			_output_types.emplace(std::piecewise_construct,
				std::forward_as_tuple(entry[0]),
				std::forward_as_tuple(io_type{ entry[1], entry_type, params }));
			break;
		}
	}
}

void io_manager::_build_inputs(std::string values_path)
{
	reader_csv reader(values_path);
	std::vector<std::string> headers;
	int record_num = 0;

	for (auto entry : reader)
	{
		if (record_num++ == 0)
		{
			//Load Headers
			headers = entry;
		}
		else
		{
			std::unordered_map<std::string, io_value> records;

			for (int i = 1; i < entry.size(); i++)
			{
				bool is_error = true;
				bool is_numeric = false;
				double numeric_value = 0;
				try
				{
					numeric_value = std::stod(entry[i]);
					is_numeric = true;
					is_error = false;
				}
				catch (std::invalid_argument ex) {
					is_error = false;
				}
				catch (std::exception ex)
				{
					is_error = true;
				}

				if (!is_error)
				{
					records.emplace(std::piecewise_construct,
						std::forward_as_tuple(headers[i]),
						std::forward_as_tuple(io_value{ is_numeric, numeric_value, entry[i] }));
				}
			}

			_inputs.emplace(std::piecewise_construct,
				std::forward_as_tuple(entry[0]),
				std::forward_as_tuple(records));

			_input_order.push_back(entry[0]);
		}
	}
}

io_type::type_enum io_manager::_type_from_string(std::string values_path)
{	
	if (values_path == "Value") return io_type::type_enum::value;
	if (values_path == "Output") return io_type::type_enum::output;
	if (values_path == "Rollover") return io_type::type_enum::rollover;
	return io_type::type_enum::invalid;
}
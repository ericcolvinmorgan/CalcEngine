#pragma once
#include <vector>
#include <unordered_map>

struct io_type {
	enum type_enum {
		value, output, rollover, invalid
	};
	
	std::string name;
	type_enum type;
	std::vector<std::string> params;
};

struct io_value {
	bool numeric;
	double value;
	std::string string_value;
};

class io_manager
{
public:
	io_manager(std::string data_types_path, std::string values_path);
	~io_manager() {};
	void update_entries(std::string entry);
	std::unordered_map<std::string, io_type> _input_types;
	std::unordered_map<std::string, io_type> _output_types;
	std::unordered_map<std::string, std::unordered_map<std::string, io_value>> _inputs;
	std::vector<std::string> _input_order;

private:
	void _build_data_types(std::string data_types_path);
	void _build_inputs(std::string values_path);
	io_type::type_enum _type_from_string(std::string values_path);
};
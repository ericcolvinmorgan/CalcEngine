#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "excel_functions_enum.h"
#include "cell_detail.h"

class cell_detail;
namespace calc_tree {
	class node
	{
	public:
		node();
		node(const nlohmann::json& json);
		virtual ~node();
		virtual std::vector<std::shared_ptr<node>> get_dependency_nodes();
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<std::string>>& value) = 0;
		virtual bool get_value(std::unordered_map<std::string, cell_detail> cell_details, std::unordered_map<std::string, excel_functions_enum> excel_functions, std::vector<std::vector<double>> &value) = 0;
		bool is_valid() { return _is_valid; };
		bool is_cell() { return _is_cell; };
		bool is_constant() { return _is_constant; };
		bool is_function() { return _is_function; };

		static std::shared_ptr<node> create_node(const nlohmann::json& json);

	protected:
		std::string _nodeType;
		bool _is_valid = false;
		bool _is_cell = false;
		bool _is_constant = false;
		bool _is_function = false;
		std::string _token;
		double _token_value = 0;
		std::shared_ptr<node> _right_node = NULL;
		std::shared_ptr<node> _left_node = NULL;
		template<typename T>
		bool _get_default_value(T& defaultValue)
		{
			if constexpr (std::is_same_v<T, std::string>) {
				defaultValue = "";
			}
			else if constexpr (std::is_same_v<T, double>) {
				defaultValue = 0;
			}
			else return false;
			return true;
		}
	};
}
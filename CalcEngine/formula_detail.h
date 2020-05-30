#pragma once
#include <string>
#include <nlohmann/json.hpp>
class formula_detail
{
public:
	enum formula_type { formula, shared_formula };
	formula_detail();
	formula_detail(const nlohmann::json& json);
	~formula_detail();

private:
	std::string _formula;
	formula_type _type;
	int _shared_index = -1;

	bool _is_master_shared = false;
};
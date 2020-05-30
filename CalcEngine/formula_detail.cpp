#include "formula_detail.h"



formula_detail::formula_detail()
{
}

formula_detail::formula_detail(const nlohmann::json& json)
{
	json.at("Formula").get_to(_formula);
	json.at("Type").get_to(_type);
	if (!json["SharedIndex"].is_null())
	{
		json.at("SharedIndex").get_to(_shared_index);
	}
	json.at("IsMasterShared").get_to(_is_master_shared);
}


formula_detail::~formula_detail()
{
}
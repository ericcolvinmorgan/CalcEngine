#include "reference.h"

reference::reference()
{
}

reference::reference(const nlohmann::json& json)
{
	json.at("Worksheet").get_to(_worksheet);
	json.at("ReferenceString").get_to(_reference_string);
	json.at("MinLocation").get_to(_min_location);
	json.at("MinRow").get_to(_min_row);
	json.at("MinRowAbsolute").get_to(_min_row_absolute);
	json.at("MinColumn").get_to(_min_column);
	json.at("MinColumnAbsolute").get_to(_min_column_absolute);
	json.at("MaxLocation").get_to(_max_location);
	json.at("MaxRow").get_to(_max_row);
	json.at("MaxRowAbsolute").get_to(_max_row_absolute);
	json.at("MaxColumn").get_to(_max_column);
	json.at("MaxColumnAbsolute").get_to(_max_column_absolute);
	json.at("NamedRange").get_to(_named_range);
}

reference::~reference()
{
	_worksheet = "d";
}
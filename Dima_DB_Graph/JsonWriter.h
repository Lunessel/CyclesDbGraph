#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <nlohmann/json.hpp>
class JsonWriter
{
public:
	static void MakeOutputArray(nlohmann::json& MainJsonList);

};

#endif
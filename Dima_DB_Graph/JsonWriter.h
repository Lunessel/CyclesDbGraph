#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <nlohmann/json.hpp>

void MakeOutputArray(std::vector<nlohmann::json>& MainJsonList, long int startindex, long int finishindex, std::string currentToken);

#endif
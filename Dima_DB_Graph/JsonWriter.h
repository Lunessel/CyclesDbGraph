#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <nlohmann/json.hpp>

void MakeOutputArray(std::vector< std::vector<nlohmann::json> >& MainJsonList, long int startindex, long int finishindex, std::string currentToken);
void MakeOutputArray_for_L_2(std::vector< std::vector<nlohmann::json> >& MainJsonList, long int startindex, long int finishindex, std::string currentToken);
#endif
#ifndef DBPARSER_H
#define DBPARSER_H

#include <string>
#include <vector>

int selectData(const char* s, std::vector<std::string> names);
int callback(void* NotUsed, int argc, char** argv, char** azColName);


#endif
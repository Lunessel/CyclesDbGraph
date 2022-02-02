#ifndef DBPARSER_H
#define DBPARSER_H

#include <string>
#include <vector>
class DbParser
{
public:

	static int selectData(const char* s, std::vector<std::string> names);
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

#endif
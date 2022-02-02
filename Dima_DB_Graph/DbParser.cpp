#include <iostream>
#include <sqlite3.h>
#include "global.h"
#include "Pair.h"
#include "DbParser.h"


int DbParser::selectData(const char* s, std::vector<std::string> names)
{
	sqlite3* DB;
	char* messageError;

	std::string sql = "";

	for (std::string name : names)
	{
		sql += "SELECT pair,token0,token1 FROM \"" + name + "\" LIMIT 100000;"; //LIMIT 5000
	}

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		std::cout << "Records selected Successfully!" << std::endl;

	//sqlite3_close(DB);
	return 0;
}

// retrieve contents of database used by selectData()
/* argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array, */
int DbParser::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	basegraph[argv[1]].push_back(Pair(argv[0], argv[1], argv[2]));
	basegraph[argv[2]].push_back(Pair(argv[0], argv[2], argv[1]));
	basegraph[argv[1]];
	basegraph[argv[2]];

	return 0;
}
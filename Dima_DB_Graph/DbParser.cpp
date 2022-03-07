#include <iostream>
#include <string>
#include <sqlite3.h>
#include "global.h"
#include "Pair.h"
#include "DbParser.h"


std::string current_exchange;


int selectData(const char* s, std::vector<std::string> names)
{
	sqlite3* DB;
	char* messageError;

	std::string sql = "";
	int exit = sqlite3_open(s, &DB);

	for (std::string name : names)
	{
		current_exchange = name;
		sql = "SELECT pair,token0,token1 FROM \"" + name + "\";"; //LIMIT 5000
		exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error in selectData function." << std::endl;
			sqlite3_free(messageError);
		}
		else
			std::cout << "Records selected Successfully!" << std::endl;
	}

	sqlite3_close(DB);
	return 0;
}

// retrieve contents of database used by selectData()
/* argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array, */
int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	basegraph[argv[1]].push_back(Pair(argv[0], argv[1], argv[2], current_exchange));
	basegraph[argv[2]].push_back(Pair(argv[0], argv[2], argv[1], current_exchange));
	basegraph[argv[1]];
	basegraph[argv[2]];

	return 0;
}

int FeeSelectData(const char* s, std::string name)
{
	sqlite3* DB;
	char* messageError;

	std::string sql = "";

	sql += "SELECT name,fee FROM \"" + name + "\";"; //LIMIT 5000

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), FeeCallback, NULL, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		std::cout << "FEE Records selected Successfully!" << std::endl;
	
	sqlite3_close(DB);
	return 0;
}

// retrieve contents of database used by selectData()
/* argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array, */
int FeeCallback(void* NotUsed, int argc, char** argv, char** azColName)
{
	feeList[argv[0]] = std::stoi(argv[1]);

	return 0;
}

int InsertIntoCycles(std::string& tokenPath, std::string& pairPath, std::string& exchanges, std::string& fees)
{
	sqlite3* DB;
	char* zErrMsg = 0;



	int exit = sqlite3_open("cycles.db", &DB);

	if (exit)
	{
		std::cout << "Can't open database: %s\n";
	}

	std::string sql = "INSERT INTO \"cycles_v0.1\" (tokenPath, pairPath, exchanges, fees) "  \
		"VALUES ('"+ tokenPath + "', '" + pairPath + "', '" + exchanges + "', '" + fees + "' ); ";
	exit = sqlite3_exec(DB, sql.c_str(), 0, 0, &zErrMsg);
	if (exit != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(DB);

	return 0;
}
/*+ std::to_string(sqlite3_last_insert_rowid(DB)) + ", '"*/
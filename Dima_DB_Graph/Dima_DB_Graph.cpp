#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <map>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "global.h"
#include "AdditionalFunctions.h"
#include "DbParser.h"
#include "DfsSearcher.h"
#include "Pair.h"
#include "JsonWriter.h"

//static int selectData(const char* s, std::vector<std::string> names);
//static int callback(void* NotUsed, int argc, char** argv, char** azColName);
//bool IsCycleUnique(const std::vector<long long int>& tempcycle);
//std::string ReplaceAll(std::string filename, const std::string& from, const std::string& to);
//void printCycles();
//void MakeOutputArray(nlohmann::json& MainJsonList);
//class Pair;

int main()
{
	dir = AdditionalFunctions::ReplaceAll("DB_Name.txt", "\\", "\\\\");
	NumberOfALLCycles = 0;
	time_t timer1, timer2;
	timer1 = time(0);
	nlohmann::json MainJsonList = nlohmann::json::array({});
	nlohmann::json json;
	std::ifstream infile("DB_Tables_Name.txt");
	std::string line;
	std::vector<std::string> result;

	while (std::getline(infile, line))
	{
		result.push_back(line);
	}

	DbParser::selectData(dir.c_str(), result); //basegraph is given
	//switch string to int in graph
	std::map<std::string, std::vector<Pair> >::iterator basegraphiterator;
	long long int vertexcounter = 0;
	long long int GraphLength = basegraph.size();
	reversedlistofvertexes.resize(GraphLength);

	for (basegraphiterator = basegraph.begin(); basegraphiterator != basegraph.end(); basegraphiterator++)
	{
		listofvertexes[basegraphiterator->first] = vertexcounter;
		reversedlistofvertexes[vertexcounter] = basegraphiterator->first;
		++vertexcounter;
	}

	graph.resize(GraphLength);

	for (basegraphiterator = basegraph.begin(); basegraphiterator != basegraph.end(); basegraphiterator++)
	{
		for (auto& i : basegraphiterator->second)
		{
			graph[listofvertexes[basegraphiterator->first]].push_back(listofvertexes[i.m_token1]);

		}
	}

	// arrays required to color the
	// graph, store the parent of node
	std::vector<long long int> color(GraphLength, 0);
	std::vector<long long int> par(GraphLength, -1);

	// call DFS to mark the cycles
	long long int firstvertex = listofvertexes["0xae13d989daC2f0dEbFf460aC112a837C89BAa7cd"]; //WBNB
	color[firstvertex] = graph[firstvertex].size() - 2;
	//--color[firstpoint];
	DfsSearcher::dfs_cycle(firstvertex, -1, color, par, firstvertex); //WBNB

	std::map<std::string, int>::iterator itColor;
	long long int RealColorSize = 0;
	for (long long int i = 0; i < color.size(); ++i)
	{
		if (color[i] != 0 || (color[i] == 0 && graph[i].size() == 1))
			++RealColorSize;
	}
	while (graph.size() != RealColorSize)
	{
		for (long long int i = 0; i < color.size(); ++i)
		{
			if (color[i] == 0 && i != firstvertex && graph[i].size() != 1)
			{
				color[i] = graph[i].size() - 2;
				DfsSearcher::dfs_cycle(i, -1, color, par, i);
				break;
			}

		}
		RealColorSize = 0;
		for (long long int i = 0; i < color.size(); ++i)
		{
			if (color[i] != 0 || (color[i] == 0 && graph[i].size() == 1))
				++RealColorSize;
		}
	}
	//printCycles();
	std::cout << cycles.size() << " was found\n";



	JsonWriter::MakeOutputArray(MainJsonList);

	//add array to json and write it to jsonfile
	json["array"] = MainJsonList;
	std::ofstream testfilejson("testjson.json");
	testfilejson << std::setw(4) << json << std::endl;
	testfilejson.close();

	std::cout << "number of ALL cycles is: " << NumberOfALLCycles << "\n";

	std::cout << "DONE!\n";

	timer2 = time(0);
	std::cout << (timer2 - timer1) << " seconds\n";

	return 0;

}

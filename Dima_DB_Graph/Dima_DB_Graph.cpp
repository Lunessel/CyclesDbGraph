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
#include <mutex>
#include "global.h"
#include "AdditionalFunctions.h"
#include "DbParser.h"
#include "DfsSearcher.h"
#include "Pair.h"
#include "JsonWriter.h"

void MakeOutputArray_for_L_2(std::vector<nlohmann::json>& MainJsonList, long int startindex, long int finishindex, std::string currentToken)
{
	std::map<std::string, std::vector<Pair> > res;

	for (auto& i : basegraph[currentToken])
	{
		res[i.m_token1].push_back(i);
	}

	std::map<std::string, std::vector<Pair> >::iterator it;
	for (it = res.begin(); it != res.end(); it++)
	{
		long int len = it->second.size();

		if (len > 1)
		{
			std::string b = it->first;
			nlohmann::json VJsonArray = nlohmann::json::array({});
			VJsonArray.push_back(currentToken);
			VJsonArray.push_back(b);
			VJsonArray.push_back(currentToken);


			for (int i = 0; i < len - 1; i++)
			{
				for (int k = i + 1; k < len; k++)
				{
					std::vector< std::pair<std::string, int> > pairArr;
					pairArr.push_back( { it->second[i].m_pair, feeList[it->second[i].m_fee] } );
					pairArr.push_back({ it->second[k].m_pair, feeList[it->second[k].m_fee] });
					
					//write arrays to json
					nlohmann::json PairsJsonArray = nlohmann::json::array({});
					//nlohmann::json ExchangesJsonArray = nlohmann::json::array({});
					
					PairsJsonArray = pairArr;

					//ExchangesJsonArray = exchanges;

					nlohmann::json j;

					j["path"] = VJsonArray;
					j["pairs"] = PairsJsonArray;
					//j["exchanges"] = ExchangesJsonArray;

					//write reverse arrays to json
					nlohmann::json ReversePairsJsonArray = nlohmann::json::array({});
					//nlohmann::json ReverseExchangesJsonArray = nlohmann::json::array({});

					std::reverse(pairArr.begin(), pairArr.end());
					//std::reverse(exchanges.begin(), exchanges.end());
					//ReverseVJsonArray = v;
	
					ReversePairsJsonArray = pairArr;
					//ReverseExchangesJsonArray = exchanges;

					nlohmann::json rj;
					rj["path"] = VJsonArray;
					rj["pairs"] = ReversePairsJsonArray;
					//rj["exchanges"] = ReverseExchangesJsonArray;

					NumberOfALLCycles += 2;

					mtx.lock();
					MainJsonList.push_back(j);
					MainJsonList.push_back(rj);
					mtx.unlock();
				}
			}
		}
	}
}

void WritingtoJsonFile_for_L_2(long int cyclesize, std::string vertex)
{
	//nlohmann::json MainJsonList = nlohmann::json::array({});
	std::vector<nlohmann::json> MainJsonList;
	nlohmann::json json;

	const int numberofthreads = 4;
	long int threadstartindex = 0;
	long int threadfinishindex = 0;
	std::vector<std::thread> threads;
	long int threadstep = cyclesize / numberofthreads;

	for (int i = 0; i < numberofthreads; ++i)
	{
		threadstartindex = threadfinishindex;
		if (i != (numberofthreads - 1))
		{
			threadfinishindex += threadstep;
		}
		else
		{
			threadfinishindex = cyclesize;
		}
		threads.push_back(std::thread(&MakeOutputArray_for_L_2, std::ref(MainJsonList), threadstartindex, threadfinishindex, vertex));
		//myThreads[i] = std::thread(&MakeOutputArray, std::ref(MainJsonList), threadstartindex, threadfinishindex);
	}
	for (auto& th : threads) {
		th.join();
	}

	//add array to json and write it to jsonfile
	json["array"] = MainJsonList;
	std::ofstream testfilejson("initialinput&output\\" + vertex + ".json");
	testfilejson << std::setw(4) << json << std::endl;
	testfilejson.close();
}

void WritingtoJsonFile(long int cyclesize, std::string vertex)
{
	//nlohmann::json MainJsonList = nlohmann::json::array({});
	std::vector<nlohmann::json> MainJsonList;
	nlohmann::json json;
	
	const int numberofthreads = 4;
	long int threadstartindex = 0;
	long int threadfinishindex = 0;
	std::vector<std::thread> threads;
	long int threadstep = cyclesize / numberofthreads;

	for (int i = 0; i < numberofthreads; ++i)
	{
		threadstartindex = threadfinishindex;
		if (i != (numberofthreads - 1))
		{
			threadfinishindex += threadstep;
		}
		else
		{
			threadfinishindex = cyclesize;
		}
		threads.push_back(std::thread(&MakeOutputArray, std::ref(MainJsonList), threadstartindex, threadfinishindex, vertex));
		//myThreads[i] = std::thread(&MakeOutputArray, std::ref(MainJsonList), threadstartindex, threadfinishindex);
	}
	for (auto& th : threads) {
		th.join();
	}

	//add array to json and write it to jsonfile
	json["array"] = MainJsonList;
	std::ofstream testfilejson("initialinput&output\\" + vertex + ".json");
	testfilejson << std::setw(4) << json << std::endl;
	testfilejson.close();
}


int main()
{
	dir = ReplaceAll("initialinput&output\\DB_Name.txt", "\\", "\\\\");
	NumberOfALLCycles = 0;
	time_t timer1, timer2;
	timer1 = time(0);

	std::ifstream infile("initialinput&output\\DB_Tables_Name.txt");
	std::string line;
	std::vector<std::string> result;

	while (std::getline(infile, line))
	{
		result.push_back(line);
	}

	FeeSelectData(std::string("base.db").c_str(), "exchange");
	selectData(dir.c_str(), result); //basegraph is given
	//switch string to int in graph
	std::map<std::string, std::vector<Pair> >::iterator basegraphiterator;
	long int vertexcounter = 0;
	long int GraphLength = basegraph.size();
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
	std::vector<long int> color(GraphLength, 0);
	std::vector<long int> par(GraphLength, -1);

	// call DFS to mark the cycles
	long int firstvertex = listofvertexes[MainToken]; //WBNB
	color[firstvertex] = graph[firstvertex].size() - 2;
	//--color[firstpoint];
	dfs_cycle(firstvertex, -1, color, par, firstvertex); //WBNB

	std::map<std::string, int>::iterator itColor;
	long int RealColorSize = 0;
	for (long int i = 0; i < color.size(); ++i)
	{
		if (color[i] != 0 || (color[i] == 0 && graph[i].size() == 1))
			++RealColorSize;
	}
	while (graph.size() != RealColorSize)
	{
		for (long int i = 0; i < color.size(); ++i)
		{
			if (color[i] == 0 && i != firstvertex && graph[i].size() != 1)
			{
				color[i] = graph[i].size() - 2;
				dfs_cycle(i, -1, color, par, i);
				break;
			}

		}
		RealColorSize = 0;
		for (long int i = 0; i < color.size(); ++i)
		{
			if (color[i] != 0 || (color[i] == 0 && graph[i].size() == 1))
				++RealColorSize;
		}
	}
	//printCycles();
	long int cyclesize = cycles.size();
	std::cout << cyclesize << " was found\n";

	//WritingtoJsonFile(cyclesize, "0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c"); //"0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c" 


	WritingtoJsonFile_for_L_2(cyclesize, "0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c"); //"0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c" 


	std::cout << "number of ALL cycles is: " << NumberOfALLCycles << "\n";

	std::cout << "DONE!\n";

	timer2 = time(0);
	std::cout << (timer2 - timer1) << " seconds\n";

	std::cin >> timer1;
	return 0;

}

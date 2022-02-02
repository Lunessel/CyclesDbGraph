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


static int selectData(const char* s, std::vector<std::string> names);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
bool IsCycleUnique(const std::vector<long long int>& tempcycle);
std::string ReplaceAll(std::string filename, const std::string& from, const std::string& to);
void printCycles();
void MakeOutputArray(nlohmann::json& MainJsonList);
class Pair;



std::string dir = ReplaceAll("DB_Name.txt", "\\", "\\\\");


std::map<std::string, std::vector<Pair> > basegraph;
std::vector<std::vector<long long int>> graph;
std::map<std::string, long long int> listofvertexes;
std::vector<std::string> reversedlistofvertexes;
std::vector<std::vector<long long int>> cycles;

long long int NumberOfALLCycles = 0;

std::vector<int> used;
int flag, n = 0;


class Pair
{
public:
	Pair(std::string pair, std::string token0, std::string token1)
		:m_pair(pair), m_token0(token0), m_token1(token1)
	{}
	~Pair() {}

	const std::string m_pair;
	const std::string m_token0;
	const std::string m_token1;
};




void dfs_cycle(long long int u, long long int p, std::vector<long long int>& color,

	std::vector<long long int>& par, long long int intputvertex)
{
	int inftemp = 0;
	bool isnotfirstvertexFlag = true;
	if (u == intputvertex && p != -1)
	{
		isnotfirstvertexFlag = false;
	}
	
	// already (completely) visited vertex.
	if (color[u] == graph[u].size()-1 && isnotfirstvertexFlag) {
		return;
	}
	

	// seen vertex, but was not completely visited -> cycle detected.
	// backtrack based on parents to find the complete cycle.
	if (color[u] > 0) {
		long long int cur = p;
		std::vector<long long int> tempcycle;

		tempcycle.push_back(cur);

		// backtrack the vertex which are
		// in the current cycle thats found
		inftemp = 0;
		bool flag = true;
		while (cur != u) {
			if (inftemp > 10)
			{
				flag = false;
				inftemp = 0;
				break;
			}
			if(cur > -1)
				cur = par[cur];
			tempcycle.push_back(cur);
			inftemp++;
		}
		if (flag)
		{
			long long int length = tempcycle.size();

			for (auto &v : graph[tempcycle[0]])
			{
				if (v == tempcycle[length - 1])
				{
					if (IsCycleUnique(tempcycle))
					{
						cycles.push_back(tempcycle);
						//printCycles();
					}
					
					return; //breakpoint
				}
			}
		}
		

	}
	par[u]=p;

	// partially visited.
	if (isnotfirstvertexFlag)
	{
		color[u] += 1;
	}

	// simple dfs on graph
	for (auto &v : graph[u]) {

		// if it has not been visited previously
		if (v == p) { // par[u]
			continue;
		}
		dfs_cycle(v, u, color, par, intputvertex);
	}
}



void printCycles()
{

	// print all the vertex with same cycle
	long long int cyclecounter = 0;
	for (auto &v : cycles)
	{
		std::cout << "Cycle Number " << cyclecounter << ": ";
		for (long long int x : cycles[cyclecounter])
			std::cout << x << " ";
		std::cout << std::endl;
		++cyclecounter;
	}
}
bool IsCycleUnique(const std::vector<long long int>& tempcycle)
{
	bool notexist = true;
	for (auto &cyc : cycles)
	{
		if (cyc.size() != tempcycle.size())
		{
			continue;
		}
		notexist = false;
		for (auto &v : cyc)
		{
			bool flag=true;
			for (auto &l : tempcycle)
			{
				if (v == l)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				return true;
			}
			
		}
		return false;

	}
	if (notexist)
	{
		return true;
	}
	return false;

}

int main()
{	
	time_t timer1,timer2;
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

	selectData(dir.c_str(), result); //basegraph is given
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
	//std::map<std::string, int > color;
	std::vector<long long int> color(GraphLength, 0);
	//std::map<std::string, std::string > par;
	std::vector<long long int> par(GraphLength, -1);
	// store the numbers of cycle

	// call DFS to mark the cycles
	long long int firstvertex = listofvertexes["0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c"]; //WBNB
	color[firstvertex] = graph[firstvertex].size() - 2;
	//--color[firstpoint];
	dfs_cycle(firstvertex, -1, color, par, firstvertex); //WBNB

	std::map<std::string, int>::iterator itColor;
	int RealColorSize = 0;
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
				color[firstvertex] = graph[firstvertex].size() - 2;
				dfs_cycle(i, -1, color, par, i);
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



	MakeOutputArray(MainJsonList);

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



std::string ReplaceAll(std::string filename, const std::string& from, const std::string& to) 
{
	std::ifstream name(filename);
	std::string str;
	std::getline(name, str);

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void MakeOutputArray(nlohmann::json& MainJsonList)
{
	std::string strsearchedtoken = "0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c";  //"0xae13d989daC2f0dEbFf460aC112a837C89BAa7cd";
	long long int searchedtoken = listofvertexes[strsearchedtoken];

	for (auto& vect : cycles)
	{
		bool isgood = false;
		int IndexofSerached = -1;
		int length = vect.size();
		for (int i = 0; i < length; ++i)
		{
			if (vect[i] == searchedtoken) // searched token
			{
				isgood = true;
				IndexofSerached = i;
				break;
			}
		}
		if (!isgood)
		{
			continue;
		}
		std::vector<long long int> v;

		for (int i = 0; i < length; ++i)
		{
			v.push_back(vect[(IndexofSerached + i) % length]);
		}

		//int length = v.size();
		std::vector<std::string> pairs;
		pairs.resize(length);
		int countpairs = 0;

		for (int i = 0; i < length; i++)
		{
			long long int first = v[i % length];
			long long int second = v[(i + 1) % length];
			if (i == length - 1)
			{
				second = v[i % length];
				first = v[0];
			}
			std::string& strfirst = reversedlistofvertexes[first];
			std::string& strsecond = reversedlistofvertexes[second];

			for (auto& p : basegraph[strsecond])
			{
				if (p.m_token1 == strfirst)
				{
					pairs[countpairs] = p.m_pair;
				}
			}
			++countpairs;

		}
		v.push_back(v[0]);
		//write arrays to json
		nlohmann::json VJsonArray = nlohmann::json::array({});
		nlohmann::json PairsJsonArray = nlohmann::json::array({});
		//VJsonArray = v;
		for (auto vel : v)
		{
			VJsonArray.push_back(reversedlistofvertexes[vel]);
		}
		PairsJsonArray = pairs;
		nlohmann::json j;
		j["path"] = VJsonArray;
		j["pairPaths"] = PairsJsonArray;
		MainJsonList.push_back(j);

		//write reverse arrays to json
		nlohmann::json ReverseVJsonArray = nlohmann::json::array({});
		nlohmann::json ReversePairsJsonArray = nlohmann::json::array({});

		std::reverse(v.begin(), v.end());
		std::reverse(pairs.begin(), pairs.end());
		//ReverseVJsonArray = v;
		for (auto vel : v)
		{
			ReverseVJsonArray.push_back(reversedlistofvertexes[vel]);
		}
		ReversePairsJsonArray = pairs;
		nlohmann::json rj;
		rj["path"] = ReverseVJsonArray;
		rj["pairPaths"] = ReversePairsJsonArray;
		MainJsonList.push_back(rj);
		NumberOfALLCycles += 2;
	}
	for (auto& v : graph[searchedtoken])
	{
		std::string strV = reversedlistofvertexes[v];
		std::string strSearchedtoken = reversedlistofvertexes[searchedtoken];
		std::string strPair;
		for (auto& p : basegraph[strV])
		{
			if (p.m_token1 == strSearchedtoken)
			{
				strPair = p.m_pair;
			}
		}

		nlohmann::json VJsonArray = nlohmann::json::array({});
		nlohmann::json PairsJsonArray = nlohmann::json::array({});
		VJsonArray.push_back(strSearchedtoken);
		VJsonArray.push_back(strV);
		VJsonArray.push_back(strSearchedtoken);

		PairsJsonArray.push_back(strPair);
		PairsJsonArray.push_back(strPair);


		nlohmann::json j;
		j["path"] = VJsonArray;
		j["pairPaths"] = PairsJsonArray;
		MainJsonList.push_back(j);
		++NumberOfALLCycles;
	}
	return;
}

static int selectData(const char* s, std::vector<std::string> names)
{
	sqlite3* DB;
	char* messageError;

	std::string sql = ""; 

	for (std::string name : names)
	{
		sql += "SELECT * FROM \"" + name + "\" LIMIT 50000;"; //LIMIT 5000
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
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{	
	basegraph[argv[3]].push_back(Pair(argv[1], argv[3], argv[5]));
	basegraph[argv[5]].push_back(Pair(argv[1], argv[5],argv[3]));
	basegraph[argv[3]];
	basegraph[argv[5]];

	return 0;
}


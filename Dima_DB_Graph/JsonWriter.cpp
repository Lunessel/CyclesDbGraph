#include "global.h"
#include "JsonWriter.h"
#include "AdditionalFunctions.h"

void MakeOutputArray(std::vector<nlohmann::json>& MainJsonList, long int startindex, long int finishindex, std::string currentToken)
{
	std::string strsearchedtoken = currentToken;
	long int searchedtoken = listofvertexes[strsearchedtoken];

	//check if cycle contain searchedtoken
	for (long int vecti = startindex; vecti < finishindex; ++vecti)
	{
		bool isgood = false;
		long int IndexofSerached = -1;
		size_t length = cycles[vecti].size();
		for (long int i = 0; i < length; ++i)
		{
			if (cycles[vecti][i] == searchedtoken) // searched token
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


		std::vector<long int> v;

		for (long int i = 0; i < length; ++i)
		{
			v.push_back(cycles[vecti][(IndexofSerached + i) % length]);
		}

		if (v.size() < 3)
		{
			continue;
		}

		v.push_back(v[0]);

		//find pairs for current cycle

		std::vector< std::vector< std::pair<std::string, int> >> paex;
		//std::vector< std::vector<std::string>> exchanges;
		//std::pair< std::vector< std::vector<std::string>>, std::vector< std::vector<std::string>> > paex;
		paex.resize(length);
		//exchanges.resize(length);
		/*pairs.resize(length);
		exchanges.resize(length);*/
		long int countpairs = 0;

		for (long int i = 0; i < length; i++)
		{
			long int first = v[i % length];
			long int second = v[(i + 1) % length];
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
					paex[countpairs].push_back( { p.m_pair, feeList[p.m_fee] } );
				}
			}
			++countpairs;

		}

	//std::vector< std::vector< std::pair<std::string, std::string> > > preparedpairs = AllPossibleComb(pairs);
	//write arrays to json
		nlohmann::json VJsonArray = nlohmann::json::array({});
		nlohmann::json PairsJsonArray = nlohmann::json::array({});
		//nlohmann::json ExchangesJsonArray = nlohmann::json::array({});
		for (auto vel : v)
		{
			VJsonArray.push_back(reversedlistofvertexes[vel]);
		}
		PairsJsonArray = paex;
		//ExchangesJsonArray = exchanges;

		nlohmann::json j;

		j["path"] = VJsonArray;
		j["pairs"] = PairsJsonArray;
		//j["exchanges"] = ExchangesJsonArray;

		//write reverse arrays to json
		nlohmann::json ReverseVJsonArray = nlohmann::json::array({});
		nlohmann::json ReversePairsJsonArray = nlohmann::json::array({});
		//nlohmann::json ReverseExchangesJsonArray = nlohmann::json::array({});

		std::reverse(v.begin(), v.end());
		std::reverse(paex.begin(), paex.end());
		//std::reverse(exchanges.begin(), exchanges.end());
		//ReverseVJsonArray = v;
		for (auto vel : v)
		{
			ReverseVJsonArray.push_back(reversedlistofvertexes[vel]);
		}
		ReversePairsJsonArray = paex;
		//ReverseExchangesJsonArray = exchanges;

		nlohmann::json rj;
		rj["path"] = ReverseVJsonArray;
		rj["pairs"] = ReversePairsJsonArray;
		//rj["exchanges"] = ReverseExchangesJsonArray;

		NumberOfALLCycles += 2;

		mtx.lock();
		MainJsonList.push_back(j);
		MainJsonList.push_back(rj);
		mtx.unlock();

	}
	return;
}


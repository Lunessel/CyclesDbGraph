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
		std::vector< std::vector< std::pair<std::string, std::string> > > pairs;
		pairs.resize(length);
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
					pairs[countpairs].push_back({ p.m_pair, p.m_fee });
				}
			}
			++countpairs;

		}

		std::vector< std::vector< std::pair<std::string, std::string> > > preparedpairs = AllPossibleComb(pairs);
		//write arrays to json
		for (auto &readypair : preparedpairs)
		{


			nlohmann::json VJsonArray = nlohmann::json::array({});
			nlohmann::json PairsJsonArray = nlohmann::json::array({});
			nlohmann::json ExchangesJsonArray = nlohmann::json::array({});
			//VJsonArray = v;
			for (auto vel : v)
			{
				VJsonArray.push_back(reversedlistofvertexes[vel]);
			}
			for (auto pe : readypair)
			{
				PairsJsonArray.push_back(pe.first);
				ExchangesJsonArray.push_back(feeList[pe.second]);
			}
			//PairsJsonArray = readypair;

			nlohmann::json j;

			j["path"] = VJsonArray;
			j["pairPath"] = PairsJsonArray;
			j["exchanges"] = ExchangesJsonArray;
			//MainJsonList.push_back(j);

			//write reverse arrays to json
			nlohmann::json ReverseVJsonArray = nlohmann::json::array({});
			nlohmann::json ReversePairsJsonArray = nlohmann::json::array({});
			nlohmann::json ReverseExchangesJsonArray = nlohmann::json::array({});

			std::reverse(v.begin(), v.end());
			std::reverse(readypair.begin(), readypair.end());
			//ReverseVJsonArray = v;
			for (auto vel : v)
			{
				ReverseVJsonArray.push_back(reversedlistofvertexes[vel]);
			}
			for (auto pe : readypair)
			{
				ReversePairsJsonArray.push_back(pe.first);
				ReverseExchangesJsonArray.push_back(feeList[pe.second]);
			}
			//ReversePairsJsonArray = readypair;
			nlohmann::json rj;
			rj["path"] = ReverseVJsonArray;
			rj["pairPath"] = ReversePairsJsonArray;
			rj["exchanges"] = ExchangesJsonArray;
			//MainJsonList.push_back(rj);
			NumberOfALLCycles += 2;

			mtx.lock();
			MainJsonList.push_back(j);
			MainJsonList.push_back(rj);
			mtx.unlock();
		}
	}
	return;
}


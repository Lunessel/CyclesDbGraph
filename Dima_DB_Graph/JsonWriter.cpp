#include "global.h"
#include "JsonWriter.h"


void MakeOutputArray(std::vector<nlohmann::json>& MainJsonList, long int startindex, long int finishindex)
{
	std::string strsearchedtoken = MainToken;
	long int searchedtoken = listofvertexes[strsearchedtoken];

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

		//int length = v.size();
		std::vector<std::string> pairs;
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
					pairs[countpairs] = p.m_pair;
				}
			}
			++countpairs;

		}
		if (v.size() < 3)
		{
			continue;
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
		//MainJsonList.push_back(j);

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
		//MainJsonList.push_back(rj);
		NumberOfALLCycles += 2;

		mtx.lock();
		MainJsonList.push_back(j);
		MainJsonList.push_back(rj);
		mtx.unlock();

	}
	return;
}


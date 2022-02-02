#include "global.h"
#include "JsonWriter.h"


void JsonWriter::MakeOutputArray(nlohmann::json& MainJsonList)
{
	std::string strsearchedtoken = "0xae13d989daC2f0dEbFf460aC112a837C89BAa7cd";  //"0xae13d989daC2f0dEbFf460aC112a837C89BAa7cd";
	long long int searchedtoken = listofvertexes[strsearchedtoken];

	for (auto& vect : cycles)
	{
		bool isgood = false;
		long long int IndexofSerached = -1;
		size_t length = vect.size();
		for (long long int i = 0; i < length; ++i)
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

		for (long long int i = 0; i < length; ++i)
		{
			v.push_back(vect[(IndexofSerached + i) % length]);
		}

		//int length = v.size();
		std::vector<std::string> pairs;
		pairs.resize(length);
		long long int countpairs = 0;

		for (long long int i = 0; i < length; i++)
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
	return;
}
#include <string>
#include "global.h"
#include "JsonWriter.h"
#include "AdditionalFunctions.h"
#include "DbParser.h"

void MakeOutputArray(long int startindex, long int finishindex, std::string currentToken)
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

		std::vector< std::vector< std::pair<std::string, std::string> >> paex;

		paex.resize(length);

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
					paex[countpairs].push_back( { p.m_pair, p.m_fee } );
				}
			}
			++countpairs;

		}

		std::string reversedtokenPath = "", tokenPath = "", pairPath = "", exchanges = "", fees = "";

		tokenPath += reversedlistofvertexes[v[0]];
		for (int i = 1; i < v.size(); ++i)
		{
			tokenPath += ", " + reversedlistofvertexes[v[i]];
		}

		std::reverse(v.begin(), v.end());

		reversedtokenPath += reversedlistofvertexes[v[0]];
		for (int i = 1; i < v.size(); ++i)
		{
			reversedtokenPath += ", " + reversedlistofvertexes[v[i]];
		}

		auto cycledata = AllPossibleComb(paex);
		for (auto data : cycledata)
		{
			pairPath = "", exchanges = "", fees = "";

			pairPath += data[0].first;
			exchanges += data[0].second;
			fees += std::to_string(feeList[data[0].second]);
			for (int i = 1; i < data.size(); ++i)
			{
				pairPath += ", " + data[i].first;
				exchanges += ", " + data[i].second;
				fees += ", " + std::to_string(feeList[data[i].second]);

			}
			mtx.lock();
			NumberOfALLCycles += 1;
			InsertIntoCycles(tokenPath, pairPath, exchanges, fees);
			mtx.unlock();


			std::reverse(data.begin(), data.end());
			pairPath = "", exchanges = "", fees = "";

			pairPath += data[0].first;
			exchanges += data[0].second;
			fees += std::to_string(feeList[data[0].second]);
			for (int i = 1; i < data.size(); ++i)
			{
				pairPath += ", " + data[i].first;
				exchanges += ", " + data[i].second;
				fees += ", " + std::to_string(feeList[data[i].second]);


			}
			mtx.lock();
			NumberOfALLCycles += 1;
			InsertIntoCycles(reversedtokenPath, pairPath, exchanges, fees);
			mtx.unlock();
		}
	}
	return;
}

void MakeOutputArray_for_L_2(long int startindex, long int finishindex, std::string currentToken)
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
			

			//std::string b = it->first;
			std::vector< std::pair<std::string, std::string> > temparr;

			for (int i = 0; i < len; i++)
			{
				temparr.push_back({ it->second[i].m_pair, it->second[i].m_fee });
			}

			std::vector< std::vector< std::pair<std::string, std::string> >> paex;

			paex.push_back(temparr);
			paex.push_back(temparr);

			std::string tokenPath = "", pairPath = "", exchanges = "", fees = "";

			tokenPath = currentToken + ", " + it->first + ", " + currentToken;

			auto cycledata = AllPossibleComb(paex);
			for (auto data : cycledata)
			{
				pairPath = "", exchanges = "", fees = "";

				pairPath += data[0].first;
				exchanges += data[0].second;
				fees += std::to_string(feeList[data[0].second]);
				for (int i = 1; i < data.size(); ++i)
				{
					pairPath += ", " + data[i].first;
					exchanges += ", " + data[i].second;
					fees += ", " + std::to_string(feeList[data[i].second]);

				}
				mtx.lock();
				NumberOfALLCycles += 1;
				InsertIntoCycles(tokenPath, pairPath, exchanges, fees);
				mtx.unlock();
			}
		}
	}
}
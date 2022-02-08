#include <string>
#include <fstream>
#include "AdditionalFunctions.h"
#include "global.h"


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

void printCycles()
{
	// print all the vertex with same cycle
	long int cyclecounter = 0;
	for (auto& v : cycles)
	{
		std::cout << "Cycle Number " << cyclecounter << ": ";
		for (long int x : cycles[cyclecounter])
			std::cout << x << " ";
		std::cout << std::endl;
		++cyclecounter;
	}
}

bool IsCycleUnique(const std::vector<long int>& tempcycle)
{
	bool notexist = true;
	for (auto& cyc : cycles)
	{
		if (cyc.size() != tempcycle.size())
		{
			continue;
		}
		notexist = false;
		for (auto& v : cyc)
		{
			bool flag = true;
			for (auto& l : tempcycle)
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

std::vector< std::vector< std::pair<std::string, std::string> > > AllPossibleComb(
	std::vector< std::vector< std::pair<std::string, std::string> > >& pairs)
{
	std::vector< std::vector< std::pair<std::string, std::string> > > res;
	std::vector< std::vector< std::pair<std::string, std::string> > > x;

	for (auto i : pairs[0])
	{
		std::vector< std::pair<std::string, std::string> > temp;
		temp.push_back(i);
		res.push_back(temp);
	}

	for (int i = 1; i < pairs.size(); ++i)
	{
		long int j = 0;
		x = res;
		long int k = res.size();

		for (int bib = 0; bib < pairs[i].size() - 1; ++bib)
		{
			for (auto t : x)
			{
				res.push_back(t);
			}
		}

		while (j < res.size())
		{
			res[j].push_back(pairs[i][j / k]);
			++j;
		}
	}

	return res;
}

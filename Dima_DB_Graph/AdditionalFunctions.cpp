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

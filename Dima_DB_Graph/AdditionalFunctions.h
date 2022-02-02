#ifndef ADDITIONALFUNCTIONS_H
#define ADDITIONALFUNCTIONS_H

#include <iostream>
#include <vector>
class AdditionalFunctions
{
public:
	static std::string ReplaceAll(std::string filename, const std::string& from, const std::string& to);
	static void printCycles();
	static bool IsCycleUnique(const std::vector<long long int>& tempcycle);


};

#endif
#ifndef ADDITIONALFUNCTIONS_H
#define ADDITIONALFUNCTIONS_H

#include <iostream>
#include <vector>

std::string ReplaceAll(std::string filename, const std::string& from, const std::string& to);
void printCycles();
bool IsCycleUnique(const std::vector<long int>& tempcycle);
std::vector< std::vector< std::pair<std::string, std::string> > > AllPossibleComb(
	std::vector< std::vector< std::pair<std::string, std::string> > >& pairs);
#endif
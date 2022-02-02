#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <map>
#include "Pair.h"
#include "AdditionalFunctions.h"

extern std::string dir;
extern long long int NumberOfALLCycles;
extern std::map<std::string, std::vector<Pair> > basegraph;
extern std::vector<std::vector<long long int>> graph;
extern std::map<std::string, long long int> listofvertexes;
extern std::vector<std::string> reversedlistofvertexes;
extern std::vector<std::vector<long long int>> cycles;
//
//class global
//{
//
//};

#endif

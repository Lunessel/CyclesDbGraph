#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include "Pair.h"
#include "AdditionalFunctions.h"

extern std::string dir;
extern long int NumberOfALLCycles;
extern std::map<std::string, std::vector<Pair> > basegraph;
extern std::vector<std::vector<long int>> graph;
extern std::map<std::string, long int> listofvertexes;
extern std::vector<std::string> reversedlistofvertexes;
extern std::vector<std::vector<long int>> cycles;
extern const std::string MainToken;
extern std::mutex mtx;
#endif

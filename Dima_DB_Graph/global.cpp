#include "global.h"

std::string dir;
long int NumberOfALLCycles;
std::map<std::string, std::vector<Pair> > basegraph;
std::vector<std::vector<long int>> graph;
std::map<std::string, long int> listofvertexes;
std::vector<std::string> reversedlistofvertexes;
std::vector<std::vector<long int>> cycles;
const std::string MainToken = "0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c";//"0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c";
std::mutex mtx;
std::map<std::string, int> feeList;
int jsoncounter = 0;
int numberofthreads;
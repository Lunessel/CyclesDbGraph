#ifndef DFSSEARCHER_H
#define DFSSEARCHER_H

#include <vector>

class DfsSearcher
{
public:
	static void dfs_cycle(long long int u, long long int p, std::vector<long long int>& color,

		std::vector<long long int>& par, long long int intputvertex);
};

#endif
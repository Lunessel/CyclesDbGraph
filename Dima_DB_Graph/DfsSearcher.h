#ifndef DFSSEARCHER_H
#define DFSSEARCHER_H

#include <vector>


void dfs_cycle(long int u, long int p, std::vector<long int>& color,

	std::vector<long int>& par, long int intputvertex);
#endif
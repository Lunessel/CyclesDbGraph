#include "global.h"
#include "AdditionalFunctions.h"
#include "DfsSearcher.h"

void dfs_cycle(long int u, long int p, std::vector<long int>& color,

	std::vector<long int>& par, long int intputvertex)
{
	int inftemp = 0;
	bool isnotfirstvertexFlag = true;
	if (u == intputvertex && p != -1)
	{
		isnotfirstvertexFlag = false;
	}

	// already (completely) visited vertex.
	if (color[u] == graph[u].size() - 1 && isnotfirstvertexFlag) {
		return;
	}

	// seen vertex, but was not completely visited -> cycle detected.
	// backtrack based on parents to find the complete cycle.
	if (color[u] > 0) {
		long int cur = p;
		std::vector<long int> tempcycle;

		tempcycle.push_back(cur);

		// backtrack the vertex which are
		// in the current cycle thats found
		inftemp = 0;
		bool flag = true;
		while (cur != u) {
			if (inftemp > 6)
			{
				flag = false;
				inftemp = 0;
				break;
			}
			if (cur > -1)
				cur = par[cur];
			tempcycle.push_back(cur);
			inftemp++;
		}
		if (flag)
		{
			long int length = tempcycle.size();

			for (auto& v : graph[tempcycle[0]])
			{
				if (v == tempcycle[length - 1])
				{
					if (IsCycleUnique(tempcycle) && tempcycle.size() > 2)
					{
						cycles.push_back(tempcycle);
					}

					return;
				}
			}
		}


	}
	par[u] = p;

	// partially visited.
	if (isnotfirstvertexFlag)
	{
		color[u] += 1;
	}

	// simple dfs on graph
	for (auto& v : graph[u]) {

		// if it has not been visited previously
		if (v == p) { // par[u]
			continue;
		}
		dfs_cycle(v, u, color, par, intputvertex);
	}
}
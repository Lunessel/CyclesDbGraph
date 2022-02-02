#ifndef PAIR_H
#define PAIR_H

#include <iostream>
class Pair
{
public:
	Pair(std::string pair, std::string token0, std::string token1);
	
	~Pair();

	const std::string m_pair;
	const std::string m_token0;
	const std::string m_token1;
};

#endif
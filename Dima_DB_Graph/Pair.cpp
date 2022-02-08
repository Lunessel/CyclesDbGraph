#include "Pair.h"


Pair::Pair(std::string pair, std::string token0, std::string token1, std::string fee)
	:m_pair(pair), m_token0(token0), m_token1(token1), m_fee(fee)
{}

Pair::~Pair() = default;
#pragma once

#include <string>
#include <array>

typedef unsigned int uint;
typedef std::pair<bool, bool> boolPair;

class STAT
{
public:
					STAT();

	void				add(bool reachable);

	std::string			address;
	uint				total();
	uint				totalOverall;
	uint				successful();
	uint				successfulOverall;
	uint				percentage();
	uint				percentageOverall();
	uint				reachable;
	uint				ID;
	uint				delayAdd;

	static const int		MAX = (100 / 5);

private:
	std::array<boolPair, MAX>	currentData;
	uint				index;
};


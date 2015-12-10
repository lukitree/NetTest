#pragma once

#include <string>

typedef unsigned int uint;

struct STAT
{
	std::string			address;
	uint				total();
	uint				totalOverall;
	uint				successful;
	uint				successfulOverall;
	uint				percentage();
	uint				percentageOverall();
	uint				reachable;
	uint				ID;
	uint				delayAdd;

	static const int		MAX = (100 / 5);
};


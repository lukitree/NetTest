#include "STAT.h"

STAT::STAT()
	: index(0)
{
	for (int i = 0; i < MAX; ++i)
	{
		boolPair pair = { false, false };
		currentData[i] = pair;
	}
}

void STAT::add(bool reachable)
{
	currentData[index] = { reachable, true };

	++index;
	if (index >= MAX)
	{
		index = 0;
	}
}

uint STAT::total()
{
	uint count = 0;

	for (auto &i : currentData)
	{
		if (i.second == true)
		{
			++count;
		}
	}

	return count;
}

uint STAT::successful()
{
	uint count = 0;

	for (auto &i : currentData)
	{
		if (i.first == true && i.second == true)
		{
			++count;
		}
	}

	return count;
}

uint STAT::percentage()
{
	return ((float)successful() / (float)total()) * 100;
}

uint STAT::percentageOverall()
{
	return ((float)successfulOverall / (float)totalOverall) * 100;
}

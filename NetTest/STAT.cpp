#include "STAT.h"

uint STAT::total()
{
	return totalOverall % (MAX + 1);
}

uint STAT::percentage()
{
	return ((float)successful / (float)total()) * 100;
}

uint STAT::percentageOverall()
{
	return ((float)successfulOverall / (float)totalOverall) * 100;
}

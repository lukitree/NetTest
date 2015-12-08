#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

const std::string VERSION = "v1.1";

struct STAT
{
	std::string					address;
	unsigned int					total() { return totalOverall % MAX; }
	unsigned int					totalOverall;
	unsigned int 					successful;
	unsigned int					successfulOverall;
	unsigned int 					percentage() { return ((float)successful / (float)total()) * 100; }
	unsigned int 					percentageOverall() { return ((float)successfulOverall / (float)totalOverall) * 100; }
	unsigned int 					reachable;
	unsigned int					ID;

private:
	const int					MAX = 100;
};

class Test
{
public:
							Test();
							~Test();

public:
	void						run();

private:
	void						ping(STAT &stat);
	void 						update(STAT &stat, bool reachable);
	void 						display();

private:
	std::vector<std::thread>			mThreads;
	//std::vector<std::string> 			addresses;
	//std::vector<unsigned int>			domainStatus;
	//std::vector<unsigned int>			pingTotal;
	//std::vector<unsigned int>			successfulPings;
	//std::vector<unsigned int>			percentages;
	std::vector<STAT>				mStats;
	std::mutex					mStatsMutex;
	bool						mStayAlive;
	bool						mUiUpdate;
	const unsigned int				cDelayLength;

};


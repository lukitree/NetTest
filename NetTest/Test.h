#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

const int MAJOR_VERSION = 1;
const int MINOR_VERSION = 20;
const std::string VERSION = "v" + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION);

struct STAT
{
	std::string					address;
	unsigned int					total() { return totalOverall % (MAX + 1); }
	unsigned int					totalOverall;
	unsigned int 					successful;
	unsigned int					successfulOverall;
	unsigned int 					percentage() { return ((float)successful / (float)total()) * 100; }
	unsigned int 					percentageOverall() { return ((float)successfulOverall / (float)totalOverall) * 100; }
	unsigned int 					reachable;
	unsigned int					ID;
	unsigned int					delayAdd;

	static const int				MAX = (100 / 5);
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
	std::vector<STAT>				mStats;
	std::mutex					mStatsMutex;
	bool						mStayAlive;
	bool						mUiUpdate;
	const unsigned int				cDelayLength;

};


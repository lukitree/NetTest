#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

#include "STAT.h"

const int MAJOR_VERSION = 1;
const int MINOR_VERSION = 22;
const std::string VERSION = "v" + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION);

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


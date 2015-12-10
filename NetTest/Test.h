#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

#include "STAT.h"

const int MAJOR_VERSION = 1;
const int MINOR_VERSION = 26;
const std::string VERSION = "v" + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION);

const uint DELAY_LENGTH = 1000;
#ifdef _DEBUG
const uint MAX_DELAY_ADD = 2000;
#else
const uint MAX_DELAY_ADD = 5000;
#endif // _DEBUG
static_assert(DELAY_LENGTH >= 1000, "DELAY_LENGTH must be atleast a second.");

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
	static const uint				cDelayLength = DELAY_LENGTH;
	static const uint				cMaxDelayAdd = MAX_DELAY_ADD;

};


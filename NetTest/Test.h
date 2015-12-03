#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

class Test
{
public:
							Test();
							~Test();

public:
	void						run();

private:
	void						ping(unsigned int domainID);
	void 						updateStatusList(const unsigned int domainID, bool reachable);
	void 						displayStatusList();

private:
	//std::mutex					StatusMutex;
	std::vector<std::thread>			threads;
	std::vector<std::string> 			addresses;
	std::vector<unsigned int>			domainStatus;
	std::vector<unsigned int>			pingTotal;
	std::vector<unsigned int>			successfulPings;
	std::vector<unsigned int>			percentages;
	bool						stayAlive;
	const unsigned int				delayLength;

};


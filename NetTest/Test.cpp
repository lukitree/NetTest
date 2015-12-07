#include "Test.h"



Test::Test()
	: delayLength(1000)
	, stayAlive(true)
{
	// Addresses to monitor
	// Can contain Domain Names or IPs
	addresses =
	{
		"192.168.1.1",
		"google.com",
		"googlr.com",
		"google.co.uk",
		"google.co.jp",
		"google.ru",

	};

	// syncronize lists to address list
	for (auto &i : addresses)
	{
		domainStatus.push_back(2);	// default "Testing" state
		pingTotal.push_back(0);
		successfulPings.push_back(0);
		percentages.push_back(0);
	}
}


Test::~Test()
{
	for (auto &i : threads)
	{
		if (i.joinable())
		{
			i.detach();
		}
	}

	std::cout << "Exiting" << std::endl;
}


void Test::run()
{
	// Create a thread for displaying the domain's status
	threads.push_back(std::thread(&Test::displayStatusList, this));

	// Create a thread for each item in domains
	for (int i = 0; i < addresses.size(); ++i)
	{
		threads.push_back(std::thread(&Test::ping, this, i));
	}

	// User presses "Enter" the quit application
	getchar();

	std::cout << "Killing running threads..." << std::endl;

	// Kill threads
	stayAlive = false;


	// detach threads
	for (auto &t : threads)
	{
		t.join();
	}
}


void Test::ping(unsigned int domainID)
{
	const std::string domainName = addresses.at(domainID);
	// Create local Windows Command for pinging domains/IPs
	const std::string command = "ping " + domainName + " -n 1 > NUL";
	bool reachable = false;

	do
	{
		// reachable
		if (system(command.c_str()) == 0)
		{
			updateStatusList(domainID, true);
			// delay ping if successful 
			Sleep(delayLength * 2);
		}
		//unreachable
		else
		{
			updateStatusList(domainID, false);
		}
	} while (stayAlive);

	std::cout << domainID << ".";
}


void Test::updateStatusList(const unsigned int domainID, bool reachable)
{
	//StatusMutex.lock();
	domainStatus[domainID] = reachable;
	++pingTotal[domainID];
	if (reachable == true)
	{
		++successfulPings[domainID];
	}
	if (successfulPings.at(domainID) != 0 && pingTotal.at(domainID))
	{
		percentages[domainID] = ((float)successfulPings.at(domainID) / (float)pingTotal.at(domainID)) * 100;
	}
	// TODO: sort results based on percentage
	//StatusMutex.unlock();
}


void Test::displayStatusList()
{
	const std::string SEPERATOR = "---------------------------------------<";

	do
	{
		// Clear Screen on each refresh
		// TODO: Only refresh when there is a change list is updated
		system("cls");

		// Program Heading
		std::cout << "      Net Tester      " << std::endl;
		std::cout << std::endl;
		std::cout << SEPERATOR;
		std::cout << std::endl;
		std::cout << std::endl;

		// Iterate through domain's list to display its statuses
		for (int i = 0; i < addresses.size(); ++i)
		{
			std::string status;
			//StatusMutex.lock();
			unsigned int reachable = domainStatus.at(i);

			switch (reachable)
			{
			case 0:	    // unreachable
				status = "-";
				break;
			case 1:	    // reachable
				status = "+";
				break;
			case 2:	    // pending testing
				status = "?";
				break;
			default:    // error
				status = "!";
			}

			//StatusMutex.unlock();

			std::cout << "  " + status + "\t" + addresses.at(i) + "\t" + "(" + std::to_string(successfulPings.at(i)) + "/" + std::to_string(pingTotal.at(i)) + ")" + " " + std::to_string(percentages.at(i)) + "%";
			std::cout << std::endl;
		}

		std::cout << std::endl;
		std::cout << SEPERATOR;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Press \"Enter\" to exit." << std::endl;

		// Don't thrash the display (too blinky)
		Sleep(delayLength);
	} while (stayAlive);
}

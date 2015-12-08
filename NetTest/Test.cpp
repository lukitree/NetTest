#include "Test.h"


Test::Test()
	: cDelayLength(2000)
	, mStayAlive(true)
	, mUiUpdate(true)
{
	// Addresses to monitor
	// Can contain Domain Names or IPs
	std::vector<std::string> addresses =
	{
		"192.168.1.1",		// Router IP
		"google.com",		// Google
		"8.8.8.8",		// Google DNS 1
		"8.8.4.4",		// Google DNS 2
		"139.130.4.5",		// Austrailian DNS
		"208.67.222.222",	// OpenDNS 1
		"208.67.220.220",	// OpenDNS 2
	};

	for (auto &i : addresses)
	{
		static int id = 0;
		STAT stat;
		stat.address = i;
		stat.totalOverall = 0;
		stat.successful = 0;
		stat.successfulOverall = 0;
		stat.reachable = 2;
		stat.ID = id;
		++id;

		mStats.push_back(stat);
	}
}


Test::~Test()
{
	for (auto &i : mThreads)
	{
		if (i.joinable())
		{
			i.join();
		}
	}

	std::cout << "Exiting" << std::endl;
}


void Test::run()
{
	// Create a thread for displaying the domain's status
	mThreads.push_back(std::thread(&Test::display, this));

	// Create a thread for each item in domains
	for (int i = 0; i < mStats.size(); ++i)
	{
		mThreads.push_back(std::thread(&Test::ping, this, std::ref(mStats[i])));
	}

	// User presses "Enter" the quit application
	getchar(); //Exit when enter is pressed

	std::cout << "Killing running threads..." << std::endl;

	// Kill threads
	mStayAlive = false;

	// detach threads
	for (auto &t : mThreads)
	{
		t.join();
	}
}


void Test::ping(STAT &stat)
{
	// Create local Windows Command for pinging domains/IPs
	const std::string command = "ping " + stat.address + " -n 1 > NUL";
	bool reachable = false;

	do
	{
		// reachable
		if (system(command.c_str()) == 0)
		{
			mStatsMutex.lock();
			update(stat, true);
			mStatsMutex.unlock();

			// normal delay if successful 
			Sleep(cDelayLength);
		}
		//unreachable
		else
		{
			mStatsMutex.lock();
			update(stat, false);
			mStatsMutex.unlock();

			// half delay if no success
			Sleep(cDelayLength / 2);
		}
	} while (mStayAlive);

	std::cout << stat.ID << " ";
}


void Test::update(STAT &stat, bool reachable)
{
	++stat.totalOverall;
	if (reachable == true)
	{
		++stat.successfulOverall;
		++stat.successful;
		stat.reachable = 1;
	}
	else
	{
		stat.reachable = 0;
	}
	if (stat.total() == 0 || stat.total() > 100)
	{
		stat.successful = 0;
	}
	mUiUpdate = true;
	// TODO: sort results based on percentage
}


void Test::display()
{
	const std::string SEPERATOR = "--------------------------------------------------------------------------------";

	do
	{
		// Clear Screen on each refresh
		if (mUiUpdate == true)
		{
			mUiUpdate = false; // reset after updating ui

			system("cls");

			// Program Heading
			std::cout << "      Net Test " << VERSION << std::endl;
			std::cout << std::endl;
			std::cout << SEPERATOR;
			std::cout << std::endl;
			std::cout << " +/-" << "\t" << "Address" << "\t\t" << "Good" << "\t" << "Total" << "\t" << "%" <<
				"\t" <<"All:\t" << "Good" << "\t" << "Total" << "\t" << "%";
			std::cout << std::endl << std::endl;

			// Iterate through domain's list to display its statuses
			for (int i = 0; i < mStats.size(); ++i)
			{
				std::string status;

				mStatsMutex.lock();
				STAT stat = mStats[i];
				mStatsMutex.unlock();
				unsigned int reachable = stat.reachable;

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

				std::cout << "  " + status + "\t";
				const int LENGTH = 15;
				int strlen = LENGTH;
				int diff = 0;
				if (stat.address.length() < LENGTH)
				{
					strlen = stat.address.length();
					diff = LENGTH - stat.address.length();
				}
				for (int s = 0; s < strlen; ++s)
				{
					std::cout << stat.address[s];
				}
				for (int space = 0; space < diff; ++space)
				{
					std::cout << " ";
				}
				std::cout << "\t" << stat.successful << "\t" << stat.total() << "\t" << stat.percentage() << "%" << "\t|\t" <<
					stat.successfulOverall << "\t" << stat.totalOverall << "\t" << stat.percentageOverall() << "%";
				std::cout << std::endl;

			}

			std::cout << std::endl;
			std::cout << SEPERATOR;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "Press \"Enter\" to exit." << std::endl;
		}
	} while (mStayAlive);
}

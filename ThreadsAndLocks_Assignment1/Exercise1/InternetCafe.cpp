#include "InternetCafe.h"

#include <iostream>
#include <thread>
#include "Shared.h"

InternetCafe::InternetCafe()
{
	mHireTimeLimit = 60;
	mHireCost = 2;
}

InternetCafe::InternetCafe(unsigned int numComputers)
{
	for (unsigned int i = 0; i < numComputers; ++i)
	{
		mComputers.push_back(new Computer(i));
	}
}

InternetCafe::~InternetCafe()
{
	for (unsigned int i = 0; i < mComputers.size(); ++i)
	{
		if (mComputers[i])
			delete mComputers[i];
	}
	mComputers.clear();
}

bool InternetCafe::HireComputer(int userId, int computerId)
{
	if (!mComputers[computerId]->IsInUse())
	{
		mComputers[computerId]->AssignUser(userId);
		return true;
	}
	else
		return false;
}

void InternetCafe::Update()
{
	for (unsigned int i = 0; i < mComputers.size(); ++i)
	{
		mComputers[i]->Update();

		if (mComputers[i]->IsInUse())
		{
			if (mComputers[i]->GetUsageTimer() >= mHireTimeLimit)
			{
				std::cout << "Unassigning user " << mComputers[i]->GetCurUserId() << " at computer " << mComputers[i]->GetId() << std::endl;
				mClientQueue.push(mComputers[i]->GetCurUserId());
				mComputers[i]->UnassignUser();
			}
		}
	}

	std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::duration<double, std::ratio<1, 1000>>(TIMESTEP)));
}

void InternetCafe::SetHireLimit(unsigned int hireLimit)
{
	mHireTimeLimit = hireLimit;
}

void InternetCafe::SetHireCost(unsigned int hireCost)
{
	mHireCost = hireCost;
}

bool InternetCafe::RequestComputer(unsigned int userId, unsigned int clientMoney, bool& clientIsRunning)
{
	//std::cout << userId << std::endl;
	//mUserQueue.push(userId);

	if (!mClientQueue.empty())
	{
		if (mClientQueue.front() == userId)
		{
			if (clientMoney >= mHireCost)
			{
				//HireComputer(userId, )
				unsigned int computerId = 0;
				bool computerFound = false;

				for (unsigned int i = 0; i < mComputers.size(); ++i)
				{
					if (!mComputers[i]->IsInUse())
					{
						computerId = mComputers[i]->GetId();
						computerFound = true;
						break;
					}
				}

				if (computerFound)
				{
					HireComputer(userId, computerId);
					std::cout << "Hired computer " << computerId << " requested by user " << userId << " (money left: " << clientMoney - mHireCost << ")" << std::endl;
					mClientQueue.pop();
					return true;
				}
			}
			else
			{
				std::cout << "User " << userId << " has insufficient money!" << std::endl;
				clientIsRunning = false;
				mClientQueue.pop();
			}
		}
	}

	return false;
}

void InternetCafe::PutInQueue(unsigned int userId)
{
	mClientQueue.push(userId);
}

const unsigned int InternetCafe::GetHireCost()
{
	return mHireCost;
}

const bool InternetCafe::HasComputersInUse()
{
	bool hasComputersInUse = false;

	for (unsigned int i = 0; i < mComputers.size(); ++i)
	{
		if (mComputers[i]->IsInUse())
		{
			hasComputersInUse = true;
			break;
		}
	}

	return hasComputersInUse;
}

const bool InternetCafe::ClientQueueIsEmpty()
{
	if (mClientQueue.size() == 0)
		return true;
	else
		return false;
}

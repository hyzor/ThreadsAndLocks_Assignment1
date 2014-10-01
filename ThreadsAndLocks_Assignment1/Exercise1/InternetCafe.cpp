#include "InternetCafe.h"

#include <iostream>


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
				//std::cout << "Unassigning user " << mComputers[i]->GetCurUserId() << " using computer " << mComputers[i]->GetId() << std::endl;
				mUserQueue.push(mComputers[i]->GetCurUserId());
				mComputers[i]->UnassignUser();
			}
		}
	}
}

void InternetCafe::SetHireLimit(unsigned int hireLimit)
{
	mHireTimeLimit = hireLimit;
}

void InternetCafe::SetHireCost(unsigned int hireCost)
{
	mHireCost = hireCost;
}

bool InternetCafe::RequestComputer(unsigned int userId, unsigned int userMoney)
{
	//std::cout << userId << std::endl;
	//mUserQueue.push(userId);

	if (!mUserQueue.empty())
	{
		if (mUserQueue.front() == userId)
		{
			if (userMoney >= mHireCost)
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
					//std::cout << "Hired computer " << computerId << " requested by user " << userId << std::endl;
					mUserQueue.pop();
					return true;
				}
			}
			else
			{
				//std::cout << "User " << userId << " has insufficient money" << std::endl;
				mUserQueue.pop();
			}
		}
	}

	return false;
}

void InternetCafe::PutInQueue(unsigned int userId)
{
	mUserQueue.push(userId);
}

const unsigned int InternetCafe::GetHireCost()
{
	return mHireCost;
}

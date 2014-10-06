#ifndef INTERNETCAFE_H_
#define INTERNETCAFE_H_

#include <vector>
#include <map>
#include <queue>

#include "Computer.h"

class InternetCafe
{
public:
	InternetCafe();
	InternetCafe(unsigned int numComputers);
	~InternetCafe();

	void PutInQueue(unsigned int userId);

	void SetHireLimit(unsigned int hireLimit);
	void SetHireCost(unsigned int hireCost);

	const unsigned int GetHireCost();

	bool RequestComputer(unsigned int userId, unsigned int clientMoney, bool& clientIsRunning);

	const bool HasComputersInUse();
	const bool ClientQueueIsEmpty();

	bool HireComputer(int userId, int computerId);
	void Update();

private:
	std::vector<Computer*> mComputers;
	//std::vector<unsigned int> mUserQueue;
	std::queue<unsigned int> mClientQueue;

	unsigned int mHireTimeLimit;
	unsigned int mHireCost;
};

#endif
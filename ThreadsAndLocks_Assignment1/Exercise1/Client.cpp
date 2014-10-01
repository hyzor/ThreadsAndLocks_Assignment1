#include "Client.h"

#include <iostream>


Client::Client()
{
	mId = -1;
	mMoney = 0;
}

Client::Client(const int id, const int money)
{
	mId = id;
	mMoney = money;
}


Client::~Client()
{
}

void Client::SetMoney(const int money)
{
	mMoney = money;
}

const int Client::GetMoney()
{
	return mMoney;
}

const int Client::GetId()
{
	return mId;
}

// void Client::Simulate()
// {
// 	while (mMoney > 0)
// 	{
// 		std::cout << "Simulating " << mId << std::endl;
// 	}
// }

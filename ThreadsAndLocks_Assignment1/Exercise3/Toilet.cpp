#include "Toilet.h"

#include <iostream>

Toilet::Toilet()
{
	mIsRunning = false;
	//mPersonIdQueue = nullptr;
	mCurClientId = -1;
	mId = -1;
}

Toilet::Toilet(int id)
	: Toilet()
{
	mId = id;
}


Toilet::~Toilet()
{
}

// void Toilet::Simulate()
// {
// 	while (mIsRunning)
// 	{
// 		std::cout << "Simulating toilet " << std::endl;
// 	}
// }

void Toilet::Init()
{
	//mPersonIdQueue = &personIdQueue;
	mIsRunning = true;
}

// bool Toilet::Request(int personId)
// {
// 	if (!HasClient())
// 	{
// 		mPersonIdQueue->pop();
// 		return true;
// 	}
// 	else
// 		return false;
// }

const int Toilet::GetCurClientId()
{
	return mCurClientId;
}

const bool Toilet::HasClient()
{
	return mCurClientId != -1;
}

const int Toilet::GetId()
{
	return mId;
}

// void Toilet::Update()
// {
// 	if (HasClient())
// 	{
// 
// 	}
// }

void Toilet::SetClient(int clientId)
{
	mCurClientId = clientId;
}

const bool Toilet::IsRunning()
{
	return mIsRunning;
}

void Toilet::Shutdown()
{
	mIsRunning = false;
}

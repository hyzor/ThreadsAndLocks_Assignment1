#include "Computer.h"


Computer::Computer()
{
	mId = -1;
	mCurUserId = -1;
	mUsageTimer = 0;
}

Computer::Computer(const unsigned int id)
	: Computer()
{
	mId = id;
}


Computer::~Computer()
{
}

void Computer::AssignUser(const unsigned int userId)
{
	mCurUserId = userId;
	mUsageTimer = 0;
}

bool Computer::IsInUse()
{
	if (mCurUserId != -1)
		return true;
	else
		return false;
}

void Computer::Update()
{
	if (IsInUse())
	{
		mUsageTimer++;
	}
}

const unsigned int Computer::GetUsageTimer()
{
	return mUsageTimer;
}

void Computer::UnassignUser()
{
	mCurUserId = -1;
}

const int Computer::GetCurUserId()
{
	return mCurUserId;
}

const unsigned int Computer::GetId()
{
	return mId;
}

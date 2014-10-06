#ifndef COMPUTER_H_
#define COMPUTER_H_

class Computer
{
public:
	Computer();
	Computer(const unsigned int id);
	~Computer();

	void AssignUser(const unsigned int userId);
	void UnassignUser();
	const bool IsInUse();

	void Update();

	const unsigned int GetUsageTimer();
	const int GetCurUserId();
	const unsigned int GetId();

private:
	int mCurUserId;
	unsigned int mUsageTimer;
	unsigned int mId;
};

#endif
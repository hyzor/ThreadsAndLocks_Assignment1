#ifndef TOILET_H_
#define TOILET_H_

#include <queue>

class Toilet
{
public:
	Toilet();
	Toilet(int id);
	~Toilet();

	void Init(std::queue<int>& personIdQueue);
	void Shutdown();

	void Simulate();

	void Update();

	void SetClient(int clientId);

	bool Request(int personId);
	const bool HasClient();

	const int GetCurClientId();
	const int GetId();

	const bool IsRunning();

private:
	bool mIsRunning;
	std::queue<int>* mPersonIdQueue;

	int mCurClientId;
	int mId;
};

#endif
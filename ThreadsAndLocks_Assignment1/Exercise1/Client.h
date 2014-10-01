#pragma once
class Client
{
public:
	Client();
	Client(const int id, const int money);
	~Client();

	//void Simulate();

	void SetMoney(const int money);
	const int GetMoney();
	const int GetId();

private:
	int mMoney;
	int mId;
};


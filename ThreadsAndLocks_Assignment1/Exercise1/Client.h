#ifndef CLIENT_H_
#define CLIENT_H_

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

#endif
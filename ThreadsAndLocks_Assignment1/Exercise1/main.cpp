#define NUM_CLIENTS 20
#define NUM_COMPUTERS 8

#define LIMIT_HIRETIME 600000
#define COST_COMPUTER 2

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <mutex>

#include "Client.h"
#include "InternetCafe.h"

void SimulateClient(InternetCafe* internetCafe, std::vector<Client*>& clients, unsigned int clientId, std::mutex& mtx)
{
	Client* curClient = nullptr;
	curClient = clients[clientId];

	if (curClient)
	{
		while (curClient->GetMoney() > 0)
		{
			//mtx.lock();
			//std::cout << "Simulating " << curClient->GetId() << std::endl;
			//mtx.unlock();

			try
			{
				mtx.lock();

				if (internetCafe->RequestComputer(curClient->GetId(), curClient->GetMoney()))
				{
					curClient->SetMoney(curClient->GetMoney() - internetCafe->GetHireCost());
				}

				mtx.unlock();
			}
			catch (std::exception& e)
			{

			}

			if (curClient->GetMoney() < internetCafe->GetHireCost())
				break;
		}

		std::cout << "User " << curClient->GetId() << " has unsufficient money left!" << std::endl;
	}
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<Client*> clients;
	InternetCafe* internetCafe;

	std::thread threads[NUM_CLIENTS];

	std::mutex mtx;

	// Random seed
	srand(time(NULL));

	// Initialize objects
	internetCafe = new InternetCafe(NUM_COMPUTERS);

	internetCafe->SetHireLimit(LIMIT_HIRETIME);
	internetCafe->SetHireCost(COST_COMPUTER);

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		int money = 0;
		money = rand() % 10 + 2;
		clients.push_back(new Client(i, money));

		//queue.push_back(clients[i]->GetId());
		internetCafe->PutInQueue(clients[i]->GetId());
	}

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		//threads[i] = std::thread(&Client::Simulate, clients[i]);
		threads[i] = std::thread(SimulateClient, internetCafe, clients, i, std::ref(mtx));
	}

	// Game loop
	while (true)
	{
		internetCafe->Update();
	}

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		threads[i].join();
	}
	
	// Clean up
	delete internetCafe;

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		if (clients[i])
			delete clients[i];
	}
	clients.clear();

	//std::cin.get();
	return 0;
}
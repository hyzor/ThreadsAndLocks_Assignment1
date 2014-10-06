#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <mutex>

#include "Client.h"
#include "InternetCafe.h"
#include "Shared.h"

void SimulateClient(InternetCafe* internetCafe, std::vector<Client*>& clients, unsigned int clientId, std::mutex& mtx)
{
	Client* curClient = nullptr;
	curClient = clients[clientId];
	bool clientIsRunning = true;

	if (curClient)
	{
		while (clientIsRunning)
		{
			if (mtx.try_lock())
			{
				if (internetCafe->RequestComputer(curClient->GetId(), curClient->GetMoney(), clientIsRunning))
				{
					curClient->SetMoney(curClient->GetMoney() - internetCafe->GetHireCost());
				}

				mtx.unlock();
			}

			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::duration<double, std::ratio<1, 1000>>(TIMESTEP)));
		}

		std::cout << "User " << curClient->GetId() << " has exited simulation" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<Client*> clients;
	clients.resize(NUM_CLIENTS);

	InternetCafe* internetCafe;

	std::thread threads[NUM_CLIENTS];

	std::mutex mtx;

	// Random seed
	srand(time(NULL));

	// Initialize objects
	internetCafe = new InternetCafe(NUM_COMPUTERS);

	// Set our hire cost and time limit for the computers
	internetCafe->SetHireLimit(LIMIT_HIRETIME);
	internetCafe->SetHireCost(COST_COMPUTER);

	// Create our client objects and immediately simulate them in their own threads
	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		int money = 0;
		money = rand() % 10 + 2;
		clients[i] = new Client(i, money);

		threads[i] = std::thread(SimulateClient, internetCafe, clients, i, std::ref(mtx));
	}

	// Try queueing all the client objects to the internet café
	unsigned int numClients = 0;
	while (numClients < NUM_CLIENTS)
	{
		if (mtx.try_lock())
		{
			internetCafe->PutInQueue(clients[numClients]->GetId());
			numClients++;

			mtx.unlock();

			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::duration<double, std::ratio<1, 1000>>(TIMESTEP)));

			std::cout << "Spawned client " << numClients << std::endl;
		}
	}

	// Internet café loop
	while (internetCafe->HasComputersInUse() || !internetCafe->ClientQueueIsEmpty())
	{
		internetCafe->Update();
	}

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		threads[i].join();
	}

	// The simulation has finished, print results
	std::cout << std::endl << "The internet cafe simulation has finished!" << std::endl;
	
	// Clean up
	delete internetCafe;

	for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
	{
		if (clients[i])
			delete clients[i];
	}
	clients.clear();

	std::cin.ignore();
	return 0;
}
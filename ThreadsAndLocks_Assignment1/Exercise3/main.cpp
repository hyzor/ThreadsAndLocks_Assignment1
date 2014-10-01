#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <future>
#include <mutex>

#include "person.hpp"
#include "Toilet.h"

#define TIMESTEP 100

void SimulatePersons(std::vector<person*>& persons, bool& appIsRunning)
{
	while (appIsRunning)
	{
		for (unsigned int i = 0; i < persons.size(); ++i)
		{
			persons[i]->waiting_time++;
		}

		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::duration<double, std::ratio<1, 1000>>(TIMESTEP)));
	}
}

void SimulateToilet(Toilet* toilet, std::queue<person*>& personQueue, std::mutex& mtx, unsigned int numJobs, bool& appIsRunning)
{
	std::vector<double> waitingTimes;

	unsigned int numJobsFinished = 0;
	unsigned int numJobsStarved = 0;

	std::cout << "Simulating toilet " << toilet->GetId() << std::endl;

	person* curPersonInProcess = nullptr;

	while (toilet->IsRunning())
	{
		if (!toilet->HasClient())
		{
			if (mtx.try_lock())
			{
				if (personQueue.size() > 0)
				{
					person* nextPerson = personQueue.front();

					// Check if the next person is not starving and will actually be able to make it in time
					if ((nextPerson->waiting_time + nextPerson->processing_time) < nextPerson->due_time)
					{
						toilet->SetClient(nextPerson->id);
						curPersonInProcess = new person(*nextPerson);
						personQueue.pop();

						waitingTimes.push_back(curPersonInProcess->waiting_time);

						std::cout << "Person " << nextPerson->id << " has entered toilet " << toilet->GetId() << std::endl;
					}
					else
					{
						numJobsStarved++;
					}
				}

				mtx.unlock();
			}
		}
		else
		{
			if (curPersonInProcess != nullptr)
			{
				curPersonInProcess->processed_time++;

				// Current person in process is done
				if (curPersonInProcess->processed_time >= curPersonInProcess->processing_time)
				{
					std::cout << "Person " << curPersonInProcess->id << " has exited toilet " << toilet->GetId() << std::endl;
					
					toilet->SetClient(-1);
					delete curPersonInProcess;
					curPersonInProcess = nullptr;

					numJobsFinished++;
				}

				std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::duration<double, std::ratio<1, 1000>>(TIMESTEP)));
			}
		}

		// All the jobs has been either finished or starved, exit
		if ((numJobsFinished + numJobsStarved) >= numJobs)
		{
			toilet->Shutdown();
			appIsRunning = false;
		}
	}

	double totalWaitingTime = 0.0;
	double averageWaitingTime = 0.0;

	for (unsigned int i = 0; i < waitingTimes.size(); ++i)
	{
		totalWaitingTime += waitingTimes[i];
	}

	averageWaitingTime = totalWaitingTime / waitingTimes.size();

	std::cout << std::endl << "Toilet simulator finished! " << std::endl << "Printing results..." << std::endl << std::endl;
	std::cout << "Finished jobs: " << numJobsFinished << std::endl;
	std::cout << "Starved jobs: " << numJobsStarved << std::endl;
	std::cout << "Average waiting time: " << averageWaitingTime << std::endl << std::endl;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	bool appIsRunning = true;

	unsigned int numPersonsToGenerate = 10;
	std::cout << "Number of persons to generate: ";
	std::cin >> numPersonsToGenerate;

	person_generator PersonGenerator;

	std::vector<person*> persons;
	std::queue<int> personIdQueue;
	//std::queue<person> mPersonQueue;
	std::queue<person*> personQueue;

	std::mutex mtx;

	Toilet* toilet = new Toilet(0);
	toilet->Init(personIdQueue);

	//std::thread personGenThread;
	std::thread personSimThread;

	personSimThread = std::thread(&SimulatePersons, std::ref(persons), std::ref(appIsRunning));

	// Consumer
	std::thread toiletThread;
	//toiletThread = std::thread(&Toilet::Simulate, toilet);
	toiletThread = std::thread(&SimulateToilet, toilet, std::ref(personQueue), std::ref(mtx), numPersonsToGenerate, std::ref(appIsRunning));

	// Producer
	while (persons.size() < numPersonsToGenerate)
	{
		if (mtx.try_lock())
		{
			person* newPerson = new person(PersonGenerator.gen_person_waiting());
			newPerson->waiting_time = 0.0;
			newPerson->processed_time = 0.0;

			persons.push_back(newPerson);
			//mPersonIdQueue.push(newPerson.id);

			// Update waiting times for all the persons
// 			for (unsigned int i = 0; i < persons.size(); ++i)
// 			{
// 				persons[i]->waiting_time++;
// 			}

			//mPersonQueue.push(newPerson);
			personQueue.push(newPerson);

			mtx.unlock();
		}
	}

// 	while (toilet->IsRunning())
// 	{
// 		for (unsigned int i = 0; i < persons.size(); ++i)
// 		{
// 			persons[i]->waiting_time++;
// 		}
// 
// 		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
// 			(std::chrono::duration<double, std::ratio<1, 1000>>(100)));
// 	}

	personSimThread.join();
	toiletThread.join();

	delete toilet;

	for (unsigned int i = 0; i < persons.size(); ++i)
	{
		if (persons[i])
			delete persons[i];
	}
	persons.clear();

	system("pause");
	//std::cin.get();
	return 0;
}
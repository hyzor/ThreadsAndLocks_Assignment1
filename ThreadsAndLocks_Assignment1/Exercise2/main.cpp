#include <iostream>
#include <thread>
#include <fstream>
#include <atomic>
#include <vector>
#include <future>
#include <mutex>

#include "timer.h"

// The numerical integration function is based off of the method explained in
// http://www.programming-techniques.com/2011/09/numerical-methods-integration-of-given.html
// written by Bibek Subedi

float y(float x)
{
	return 4 / (1 + x * x);
}

void ThreadFunc(std::mutex& mtx, float& s, float x0, float h, float numTrapezes, unsigned int threadId, unsigned numThreads)
{
	float curTrapeze = (float)threadId + 1;
	float trapezeTotals = 0.0f;

	// First check if thread is valid
	if (curTrapeze < numTrapezes)
	{
		// Thread calculates all the trapezes corresponding to this thread ID (and the trapezes that are a multiplier of its ID)
		while (curTrapeze < numTrapezes)
		{
			trapezeTotals += 2 * y(x0 + curTrapeze * h);
			curTrapeze += numThreads;
		}

		bool hasFinished = false;

		while (!hasFinished)
		{
			// Now try to increase s (the global trapeze value)
			if (mtx.try_lock())
			{
				s += trapezeTotals;

				hasFinished = true;
				mtx.unlock();
			}
		}
	}
}

void ThreadFunc_Start(float& s, float& h, float x0, float xn, float numTrapezes)
{
	h = y(xn - x0) / numTrapezes;
	s = y(x0) + y(xn);
}

void SerialFunc(float &s, float h, float curTrapeze, float x0, float xn)
{
	s += 2 * y(x0 + curTrapeze * h);
}

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		std::cout << std::atoi(argv[i]) << std::endl;
	}

	std::ofstream myFile;
	myFile.open("Results.txt", std::ios::app);

	unsigned int numThreads = 8;
	unsigned int numTrapezes = 1024 * 1024;

	if (argc > 2)
	{
		int arg_numThreads = std::atoi(argv[1]);
		int arg_numTrapezes = std::atoi(argv[2]);

		numThreads = arg_numThreads; 
		numTrapezes = arg_numTrapezes;
	}

	std::cout << "Executing with " << numThreads << " threads and " << numTrapezes << " trapezes" << std::endl;

	Timer timer;

	float x0 = 0;
	float xn = 1;
	float h;
	float s;

	std::thread* myThreads;
	myThreads = new std::thread[numThreads];

	std::mutex mtx;

	ThreadFunc_Start(s, h, x0, xn, (float)numTrapezes);

	timer.Start();

	if (numThreads > 0)
	{
		for (unsigned int i = 0; i < numThreads; ++i)
		{
			myThreads[i] = std::thread(ThreadFunc, std::ref(mtx), std::ref(s), x0, h, numTrapezes, i, numThreads);
		}
	}
	else
	{
		for (unsigned int i = 1; i < numTrapezes; ++i)
		{
			SerialFunc(std::ref(s), h, (float)i, x0, xn);
		}
	}

	for (unsigned int i = 0; i < numThreads; ++i)
	{
		myThreads[i].join();
	}

	float integralValue = (h * 0.5f) * s;
	timer.Stop();

	std::cout << "Integral value: " << integralValue << std::endl;
	std::cout << "Calculated in " << timer.Elapsed() << "ms" << std::endl;

	if (myFile.is_open())
	{
		myFile << numThreads << " " << numTrapezes << " : " << integralValue << " " << timer.Elapsed() << "ms" << std::endl;
	}

	return 0;
}
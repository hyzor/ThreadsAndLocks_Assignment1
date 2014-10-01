#include <iostream>
#include <thread>

#include "timer.h"

// float TrapezeFunction(float x)
// {
// 
// }

float y(float x)
{
	return 4 / (1 + x * x);
}

void ThreadFunc(float& s, float x0, float i, float h, unsigned int& numTrapezesCalculated)
{
	s += 2 * y(x0 + i * h);
	numTrapezesCalculated++;
}

int main()
{
	unsigned int numThreads = 0;
	unsigned int numTrapezes = 16;

	std::cout << "Threads: ";
	std::cin >> numThreads;

	std::cout << "Trapezes: ";
	std::cin >> numTrapezes;

	std::cout << "Executing with " << numThreads << " threads and " << numTrapezes << " trapezes" << std::endl;

	Timer timer;

	float x0 = 0;
	float xn = 1;
	float h, s;

	timer.Start();
	h = y(xn - x0) / numTrapezes;
	s = y(x0) + y(xn);

	std::thread* myThreads;
	myThreads = new std::thread[numThreads];

	unsigned int numTrapezesCalculated = 0;

	// There are one or more threads available for every trapeze
	if (numThreads >= numTrapezes - 1)
	{
		for (unsigned int i = 1; i < numTrapezes; ++i)
		{
			myThreads[i-1] = std::thread(ThreadFunc, std::ref(s), x0, i, h, std::ref(numTrapezesCalculated));
		}

		for (unsigned int i = 1; i < numTrapezes; ++i)
		{
			myThreads[i-1].join();
		}
	}

	else if (numThreads > 0)
	{
		// There is an even number of threads in proportion to the number of trapezes
		// but there are less threads than the number of trapezes
		if ((numTrapezes - 1) % numThreads == 0)
		{
			unsigned int numIterations = (numTrapezes - 1) / numThreads;

			for (unsigned int i = 0; i < numIterations; ++i)
			{
				for (unsigned int j = 0; j < numThreads; ++j)
				{
					myThreads[j] = std::thread(ThreadFunc, std::ref(s), x0, ((i * numThreads) + j) + 1, h, std::ref(numTrapezesCalculated));
				}

				for (unsigned int j = 0; j < numThreads; ++j)
				{
					myThreads[j].join();
				}
			}
		}

		// There are more trapezes than there are threads and the threads
		// are in an odd number in proportion to the number of trapezes (and there are threads to be executed)
		else
		{
			unsigned int curTrapeze = 1;

			while (numTrapezesCalculated < (numTrapezes - 1))
			{
				unsigned int trapezesLeft = (numTrapezes - 1) - numTrapezesCalculated;

				// There are less trapezes left to calculate than the total amount of threads
				if (trapezesLeft < numThreads)
				{
					for (unsigned int i = 0; i < trapezesLeft; ++i)
					{
						myThreads[i] = std::thread(ThreadFunc, std::ref(s), x0, curTrapeze, h, std::ref(numTrapezesCalculated));
						curTrapeze++;
					}

					for (unsigned int i = 0; i < trapezesLeft; ++i)
					{
						myThreads[i].join();
					}
				}

				// There are still more trapezes left to calculate than the total amount of threads
				else
				{
					for (unsigned int i = 0; i < numThreads; ++i)
					{
						myThreads[i] = std::thread(ThreadFunc, std::ref(s), x0, curTrapeze, h, std::ref(numTrapezesCalculated));
						curTrapeze++;
					}

					for (unsigned int i = 0; i < numThreads; ++i)
					{
						myThreads[i].join();
					}
				}
			}
		}
	}

	// No threads to be executed
	else
	{
		for (unsigned int i = 1; i < numTrapezes; ++i)
		{
			ThreadFunc(s, x0, i, h, numTrapezesCalculated);
		}
	}

// 	for (unsigned int i = 1; i < numTrapezes; ++i)
// 	{
// 		//s += 2 * y(x0 + i * h);
// 		ThreadFunc(s, x0, i, h);
// 	}

	float integralValue = (h * 0.5f) * s;
	timer.Stop();

	std::cout << "Integral value: " << integralValue << std::endl;
	std::cout << "Calculated in " << timer.Elapsed() << "ms" << std::endl;
	system("pause");

	return 0;
}
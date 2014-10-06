#ifndef PERSON_HPP_
#define PERSON_HPP_

#include<chrono>
#include<random>
#include<thread>

/* time is represented using doubles */
using simulation_time_t = double;

/* some random number generators for creating persons */
std::random_device rd;
std::default_random_engine generator(rd());
std::exponential_distribution<> next_arrival_time(1.0/100);
std::normal_distribution<> next_due_time(500, 150);
std::normal_distribution<> next_processing_time(100, 25);

/* person class, constructor takes the global time of previous person arriving, initially 0 */
class person {
	public:
		int id;
		simulation_time_t arrival_time;
		simulation_time_t due_time;
		simulation_time_t processing_time;
		simulation_time_t waiting_time;
		simulation_time_t processed_time;

		person(int id, simulation_time_t current_time) :
			id(id),
			arrival_time(current_time + next_arrival_time(generator)),
			due_time(arrival_time + next_due_time(generator)),
			processing_time(next_processing_time(generator)),
			waiting_time(-1.0), // initialize to nonsense value
			processed_time(-1.0) // initialize to nonsense value
		{}

		/* copy constructor */
		person(const person& p) :
			id(p.id),
			arrival_time(p.arrival_time),
			due_time(p.due_time),
			processing_time(p.processing_time),
			waiting_time(p.waiting_time),
			processed_time(p.processed_time)
		{}
};

class person_generator {
	int last_id;
	simulation_time_t last_arrival_time;

	public:
		person_generator() : last_id(0), last_arrival_time(0) {}

		/* generate a person without slowing down execution; time synchronization is achieved entirely in YOUR code */
		person gen_person_nowaiting() {
			person p(++last_id, last_arrival_time);
			last_arrival_time = p.arrival_time;
			return p;
		}

		/* generate a person and wait until he/she arrives; time synchronization is achieved using a similar waiting call in the consumer */
		person gen_person_waiting() {
			person p(++last_id, last_arrival_time);

			// Chrono doesn't work properly in Visual Studio 2013, so I had to add a duration_cast to the sleep function
			//std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(p.arrival_time - last_arrival_time)); // VS 2013 bug
			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::duration<double, std::ratio<1, 1000>>(p.arrival_time - last_arrival_time)));

			std::cout << "Generated person " << p.id << std::endl;

			last_arrival_time = p.arrival_time;
			return p;
		}
};

#endif
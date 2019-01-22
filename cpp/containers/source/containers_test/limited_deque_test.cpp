#include <iostream>

#include "containers/limited_queue.hpp"

void limited_deque_test()
{
	containers::limited_queue<int> lq(5);

	// push limit:
	{
		int counter = 0;
		while (lq.push(counter))
		{
			std::cout << "Pushed " << counter << " to the back of queue\n";
			++counter;
		}

		std::cout << "\nReached queue limit at " << lq.size() << "\n\n";
	}


	// lowering the limit:
	{
		std::cout << "Changing queue limit to 3\n";
		lq.limit(3);
		std::cout << "Size of the queue is " << lq.size() << "\n";
		std::cout << "First element is " << lq.front() << " and last is " << lq.back() << "\n\n";
	}
}

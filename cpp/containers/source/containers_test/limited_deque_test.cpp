#include <iostream>

#include "containers/limited_queue.hpp"

void limited_deque_test()
{
	containers::limited_queue<int> lq(3);

	int counter = 0;
	while (lq.push(counter))
	{
		std::cout << "Pushed " << counter << " to the back of queue\n";
		++counter;
	}

	std::cout << "\nReached queue limit at " << lq.size() << "\n\n";
}

#include <iostream>

#include "containers/queue/nonuniform_limited_queue.hpp"

void nonuniform_limited_queue_test()
{
	containers::nonuniform_limited_queue<int> nlq(6);

	// pushed into queue until volume_limit is reached:
	{
		auto push = [&nlq](int value, std::size_t size)
		{
			if (nlq.push(value, size))
			{
				std::cout << "Pushed " << value << " into queue with size " << size << "\n";
			}
			else
			{
				std::cout << "Failed to push " << value << " into queue with size " << size << "\n";
			}
		};
	
		push(0, 1);
		push(1, 3);
		push(2, 4);
		push(3, 1);

		std::cout << "Queue size limit is " << nlq.volume_limit() << "\n\n";
	}

	// lower queue volume_limit:
	{
		std::cout << "Changing queue limit to 3\n";
		nlq.volume_limit(3);
		std::cout << "First element is " << nlq.front() << " and last is " << nlq.back() << "\n\n";
	}
}

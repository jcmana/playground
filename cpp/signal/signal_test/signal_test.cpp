#include <iostream>

#include "../signal/signal.h"

int main()
{
	binary_signal a(4.0, 6.0);
	binary_signal b(1.0, 15.0);
	binary_signal c;;

	auto not_a = ~a;
	auto merge = a & b;

	for (const auto & edge : merge.m_edges)
	{
		std::cout << edge.first << std::endl;
	}

	return 0;
}
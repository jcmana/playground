#include "basic_graph.h"

int main()
{
	graph<int, int, unsigned int> g;

	g.create(0, 26);
	g.create(1, 13);
	g.create(2, 7);

	g.link(0, 1, 103);

	return 0;
}
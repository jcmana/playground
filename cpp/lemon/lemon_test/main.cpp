#include <iostream>
#include <lemon/list_graph.h>

enum class EdgeType
{
	up, down,
	above, below
};

int main()
{
	lemon::ListDigraph g;

	lemon::ListDigraph::Node u = g.addNode();
	lemon::ListDigraph::Node v = g.addNode();
	lemon::ListDigraph::Arc  a = g.addArc(u, v);

	lemon::ListDigraph::ArcMap<EdgeType> edgeTypes(g);

	edgeTypes[a] = EdgeType::down;

	for (lemon::ListDigraph::ArcIt n(g); n != lemon::INVALID; ++n)
	{
		std::string edgeTypeStr;

		switch (edgeTypes[n])
		{
			case EdgeType::up: edgeTypeStr = "up"; break;
			case EdgeType::down: edgeTypeStr = "down"; break;
		}

		std::cout << "<- " << edgeTypeStr << std::endl;
	}
	
	return 0;
}

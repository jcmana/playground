#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "../../generic/generic/bidirectional_map.hpp"

#include "containers/graph/node_centric.hpp"
#include "containers/graph/ordering/graph_preorder.hpp"

void graph_test()
{
	std::cout << "graph test:" << std::endl;

	if (true)
	{
		using graph = containers::graph::node_centric<std::string, int>;

		graph g;

		static constexpr int EDGE_SREF = 1;
		static constexpr int EDGE_AREF = 2;

		generic::bidirectional_map<int, std::string> emap(
		{
			{ EDGE_SREF, "sref" },
			{ EDGE_AREF, "aref" }
		});

		// add nodes
		graph::node * struct_1 = g.create_node("struct_1");
		graph::node * struct_2 = g.create_node("struct_2");
		graph::node * struct_3 = g.create_node("struct_3");
		graph::node * struct_4 = g.create_node("struct_4");
		graph::node * struct_5 = g.create_node("struct_5");
		
		// add edges
		g.create_edge(struct_1, struct_3, EDGE_SREF);
		g.create_edge(struct_1, struct_2, EDGE_SREF);
		g.create_edge(struct_2, struct_3, EDGE_SREF);
		g.create_edge(struct_3, struct_4, EDGE_SREF);
		//g.create_edge(struct_3, struct_1, EDGE_SREF);

		for (auto ec : preorder(&g, struct_1))
		{
			std::cout << ec->source->property << " -- " << emap.atob(ec->property) << " -> " << ec->target->property << std::endl;
		}
	}

	std::cout << std::endl;
	
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

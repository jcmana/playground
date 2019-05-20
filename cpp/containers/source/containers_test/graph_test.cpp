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


		graph_preorder_iterator<graph> itp;

		// Preorder traversal:
		{
			for (graph::edge * edge_ptr : preorder(&g, struct_1))
			{
				std::cout << edge_ptr->source->property << " -- " << emap.atob(edge_ptr->property) << " -> " << edge_ptr->target->property << std::endl;
			}
		}

		// Iterator copy/move:
		{
			auto g_preorder = preorder(&g, struct_1);

			auto it = g_preorder.begin();
			++it;

			auto it_copy = it;
			it_copy++;

			auto it_move = std::move(it);

			//std::cout << "it:      " << it->source->property << " -> " << it->target->property << std::endl;
			std::cout << "it_copy: " << it_copy->source->property << " -> " << it_copy->target->property << std::endl;
			std::cout << "it_move: " << it_move->source->property << " -> " << it_move->target->property << std::endl;
		}

	}

	std::cout << std::endl;
	
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

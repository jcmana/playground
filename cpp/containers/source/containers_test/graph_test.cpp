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
#include "containers/graph/iterators/preorder_node_iterator.hpp"
#include "containers/graph/ordering/preordered.hpp"

void graph_test()
{
	std::cout << "graph test:" << std::endl;

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
	graph::node * root = g.create_node("root");
	graph::node * struct_1 = g.create_node("struct_1");
	graph::node * struct_2 = g.create_node("struct_2");
	graph::node * struct_3 = g.create_node("struct_3");
	graph::node * struct_4 = g.create_node("struct_4");
	graph::node * struct_5 = g.create_node("struct_5");
	graph::node * struct_6 = g.create_node("struct_6");
		
	// add edges
	g.create_edge(root, struct_1, EDGE_SREF);
	g.create_edge(root, struct_2, EDGE_SREF);
	g.create_edge(struct_1, struct_4, EDGE_SREF);
	g.create_edge(struct_1, struct_3, EDGE_SREF);
	g.create_edge(struct_3, struct_4, EDGE_SREF);
	g.create_edge(struct_4, struct_5, EDGE_SREF);
	g.create_edge(struct_5, struct_6, EDGE_SREF);
	//g.create_edge(struct_5, struct_1, EDGE_SREF);			// cyclic edge

	// Preorder traversal:
	if (false)
	{
		for (graph::edge * edge_ptr : preorder(&g, root))
		{
			std::cout << edge_ptr->source->property << " -- " << emap.atob(edge_ptr->property) << " -> " << edge_ptr->target->property << std::endl;
		}
	}

	// Iterator copy/move:
	if (false)
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

	// Node iterator:
	if (true)
	{
		containers::graph::preorder_node_iterator<graph> it(root);
		containers::graph::preorder_node_iterator<graph> it_end;
		for (; it != it_end; ++it)
		{
			std::cout << it->property << std::endl;

			for (graph::edge * edge_ptr : it.path())
			{
				std::cout << "   " << edge_ptr->source->property << " -> " << edge_ptr->target->property << std::endl;
			}
		}
	}


	std::cout << std::endl;
	
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

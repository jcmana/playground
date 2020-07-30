#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <utility>

#include "../../generic/generic/bidirectional_map.hpp"

#include "containers/graph/node_centric.hpp"
#include "containers/graph/cursors/path_cursor.hpp"
#include "containers/graph/iterators/preorder_edge_iterator.hpp"
#include "containers/graph/iterators/preorder_node_iterator.hpp"
#include "containers/graph/iterators/preorder_path_iterator.hpp"
#include "containers/graph/iterators/postorder_path_iterator.hpp"
#include "containers/graph/search/dijkstra.hpp"
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
	graph::edge * r_s1 = g.create_edge(root, struct_1, EDGE_SREF);
	graph::edge * r_s2 = g.create_edge(root, struct_2, EDGE_SREF);
	g.create_edge(struct_1, struct_3, EDGE_SREF);
	g.create_edge(struct_1, struct_4, EDGE_SREF);
	g.create_edge(struct_3, struct_4, EDGE_SREF);
	//g.create_edge(struct_4, struct_5, EDGE_SREF);
	//g.create_edge(struct_5, struct_6, EDGE_SREF);
	//g.create_edge(struct_5, struct_1, EDGE_SREF);			// cyclic edge
    g.create_edge(struct_2, struct_5, EDGE_SREF);

    // Path cursor:
    if (false)
    {
        using namespace containers::graph;

        graph::edge * edge_ptr = nullptr;

		path_cursor<graph> c({r_s1});

        while (c.empty() == false)
        {
            if (c.match())
            {
                std::cout << c->back()->source->property << "->" << c->back()->target->property << "\n";
                c.consume();
            }
            else
            {
                c.expand();
            }
        }
    }

	// Node iterator:
	if (false)
	{
		containers::graph::preorder_node_iterator<graph> it(root);
		containers::graph::preorder_node_iterator<graph> it_end;
		for (; it != it_end; ++it)
		{
			std::cout << it->property << std::endl;
		}
	}

	// Preorder path iterator:
	if (true)
	{
		using namespace containers::graph;

		preorder_path_iterator<graph> it(r_s1);
		preorder_path_iterator<graph> it_end;

		for (; it != it_end; ++it)
		{
			std::cout << it->back()->target->property << ": ";
			for (const graph::edge * edge_ptr : *it)
			{
				std::cout << edge_ptr->source->property << "->" << edge_ptr->target->property << " ";
			}

			std::cout << std::endl;
		}
	}

	// Postorder path iterator:
	if (false)
	{
		using namespace containers::graph;

		postorder_path_iterator<graph> it(root);
		postorder_path_iterator<graph> it_end;

		for (; it != it_end; ++it)
		{
			std::cout << it->back()->source->property << "->" << it->back()->target->property << "\n";
		}
	}

	// Dijkstra search:
	if (false)
	{
		auto path = containers::graph::dijkstra<graph>(g, root, struct_5);

		for (const graph::node * node_ptr : path)
		{
			std::cout << node_ptr->property << "\n";
		}
	}

	std::cout << std::endl;
}

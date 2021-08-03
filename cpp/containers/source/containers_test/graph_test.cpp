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
#include "containers/graph/cursors/path_cursor_generic.hpp"
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
	auto * root = g.create_node("root");
	auto * struct_1 = g.create_node("struct_1");
	auto * struct_2 = g.create_node("struct_2");
	auto * struct_3 = g.create_node("struct_3");
	auto * struct_4 = g.create_node("struct_4");
	auto * struct_5 = g.create_node("struct_5");
	auto * struct_6 = g.create_node("struct_6");
	
	// add edges
	auto * r_s1 = g.create_edge(root, struct_1, EDGE_SREF);
	auto * r_s2 = g.create_edge(root, struct_2, EDGE_SREF);
	auto * s1_s3 = g.create_edge(struct_1, struct_3, EDGE_SREF);
	auto * s1_s4 = g.create_edge(struct_1, struct_4, EDGE_SREF);
	g.create_edge(struct_3, struct_4, EDGE_SREF);
	//g.create_edge(struct_4, struct_5, EDGE_SREF);
	//g.create_edge(struct_5, struct_6, EDGE_SREF);
	//g.create_edge(struct_5, struct_1, EDGE_SREF);			// cyclic edge
    g.create_edge(struct_2, struct_5, EDGE_SREF);

    // Path cursor (postorder traversal):
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

	// Path cursor generic (postorder traversal):
	if (true)
	{
		using namespace containers::graph::experimental;

		auto expand = [](auto & c)
		{
			auto * curr_edge_ptr = c.top();
			auto * curr_node_ptr = curr_edge_ptr->target;

			std::vector<graph::edge *> expansion;
			for (auto it = curr_node_ptr->outgoing.rbegin(); it < curr_node_ptr->outgoing.rend(); ++it)
			{
				expansion.push_back(*it);
			}

			c.push(curr_edge_ptr, expansion);
		};

		graph::edge * edge_ptr = nullptr;

		//path_cursor<graph::edge *> c;
		//c.push(r_s1, {r_s1, s1_s4, s1_s3});
		
		path_cursor<graph::edge *> c({r_s2, r_s1});

		while (c.match() == false)
		{
			expand(c);
		}

		while (c.empty() == false)
		{
			if (c.match())
			{
				std::cout << c->back()->source->property << "->" << c->back()->target->property << "\n";
				c.pop();
			}
			else
			{
				expand(c);
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
	if (false)
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

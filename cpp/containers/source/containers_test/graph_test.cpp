#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <algorithm>
#include <utility>

#include "../../generic/generic/bidirectional_map.hpp"

#include "containers/graph/node_centric.hpp"
#include "containers/graph/cursors/preorder_path_cursor.hpp"
#include "containers/graph/cursors/postorder_path_cursor.hpp"
#include "containers/graph/iterators/preorder_edge_iterator.hpp"
#include "containers/graph/iterators/preorder_node_iterator.hpp"
#include "containers/graph/iterators/preorder_path_iterator.hpp"
#include "containers/graph/iterators/postorder_edge_iterator.hpp"
#include "containers/graph/search/dijkstra.hpp"
#include "containers/graph/ordering/preordered.hpp"

void graph_test()
{
	std::cout << "graph test:" << std::endl;

	using graph = containers::graph::node_centric<std::string, int>;

	// Graph container
	graph g;

	static constexpr int EDGE_SREF = 1;
	static constexpr int EDGE_AREF = 2;

	// add nodes
	auto struct_r = g.create_node("struct_root");
	auto struct_1 = g.create_node("struct_1");
	auto struct_2 = g.create_node("struct_2");
	auto struct_3 = g.create_node("struct_3");
	auto struct_4 = g.create_node("struct_4");
	auto struct_5 = g.create_node("struct_5");
	auto struct_6 = g.create_node("struct_6");
	
	// add edges
	auto r_s1 = g.create_edge(struct_r, struct_1, EDGE_SREF);
	auto r_s2 = g.create_edge(struct_r, struct_2, EDGE_SREF);
	g.create_edge(struct_1, struct_3, EDGE_SREF);
	g.create_edge(struct_1, struct_4, EDGE_SREF);
	g.create_edge(struct_3, struct_4, EDGE_SREF);
	//g.create_edge(struct_4, struct_5, EDGE_SREF);
	//g.create_edge(struct_5, struct_6, EDGE_SREF);
	//g.create_edge(struct_5, struct_1, EDGE_SREF);			// cyclic edge
	g.create_edge(struct_2, struct_5, EDGE_SREF);

	/*
    // Cursor:
    if (false)
    {
        using namespace containers::graph;

        graph::edge * edge_ptr = nullptr;

        postorder_path_cursor<graph> c(r_s1);

        while (c.empty() == false)
        {
            if (c.valid())
            {
                std::cout << c->source->property << "->" << c->target->property << "\n";
                c.consume();
            }
            else
            {
                c.expand();
            }
        }
    }

    // Postorder edge iterator:
    if (true)
    {
        using namespace containers::graph;

        postorder_edge_iterator<graph> it(r_s1);
        postorder_edge_iterator<graph> it_end;

        for (; it != it_end; ++it)
        {
            std::cout << it->source->property << "->" << it->target->property << "\n";
        }
    }

    if (false)
    {
        using namespace containers::graph;

        preorder_path_cursor<graph> c;
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

	// Path iterator:
	if (false)
	{
		containers::graph::preorder_path_iterator<graph> it(r_s1);
		containers::graph::preorder_path_iterator<graph> it_end;

		for (; it != it_end; ++it)
		{
			auto path = *it;

			std::cout << path.back()->target->property << ": ";
			for (const graph::edge * edge_ptr : path)
			{
				std::cout << edge_ptr->source->property << "->" << edge_ptr->target->property << " ";
			}

			std::cout << std::endl;
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
	*/

	if (false)
	{
		using namespace std;

		enum 
		{
			No,
			Lo,
		};

		using stack_node = graph::edge;
		using stack = vector<stack_node>;

		stack s;

		s.emplace_back(r_s2);
		s.emplace_back(r_s1);

		while (s.empty() == false)
		{
			auto c = s.back();
			s.pop_back();

			cout << g[g[c].source].property << " -> " << g[g[c].target].property << endl;

			for (auto o : g[g[c].target].outgoing)
			{
				s.emplace_back(o);
			}
		}
	}

	if (true)
	{
		using namespace std;

		enum
		{
			No,
			Lo,
		};

		using stack_node = graph::edge;
		using stack = vector<stack_node>;

		stack e;
		stack d;

		e.emplace_back(r_s2);
		e.emplace_back(r_s1);

		do
		{
			auto c = e.back();

			for (auto o : g[g[c].target].outgoing)
			{
				e.emplace_back(o);
			}

			d.emplace_back(c);
		}
		while (e.back().offset != d.back().offset);

		cout << g[g[d.back()].source].property << " -> " << g[g[d.back()].target].property << endl;
	}
}

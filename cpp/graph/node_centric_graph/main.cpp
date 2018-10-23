#include <iostream>
#include <algorithm>
#include <string>

#include "node_centric.hpp"

int main()
{
	if (true)
	{
		using graph = node_centric_graph<std::string, int>;
		graph g;

		// add nodes
		graph::nc_node * a = g.create_node("node a");
		graph::nc_node * b = g.create_node("node b");
		graph::nc_node * c = g.create_node("node c");

		// add edges
		graph::nc_edge * u = g.create_edge(a, b, 3);
		graph::nc_edge * v = g.create_edge(a, c, 7);

		// find node with property
		auto & res = std::find_if(g.nodes.begin(), g.nodes.end(), [](const auto & ptr) -> bool
		{
			return (ptr->property == "node a");
		});

		if (res != g.nodes.end())
		{
			std::cout << "found " << res->get()->property << std::endl;
		}

		// print graph
		for (const auto & node_it : g.nodes)
		{
			std::cout << node_it->property << std::endl;

			for (const auto & edge_it : node_it->incoming)
			{
				std::cout << "   <- " << edge_it->source->property << "(" << edge_it->property << ")" << std::endl;
			}

			for (const auto & edge_it : node_it->outgoing)
			{
				std::cout << "   -> " << edge_it->target->property << "(" << edge_it->property << ")" << std::endl;
			}
		}
	}

	if (true)
	{
		struct element
		{
			explicit element(int t) : type(t) {}
			int type;
		};

		struct polygon :
			element
		{
			explicit polygon(double p) : element(3), point(p) {}
			double point;
		};

		using sp_graph = node_centric_graph<std::shared_ptr<element>, int>;
		sp_graph f;

		auto * a = f.create_node(std::move(std::make_shared<polygon>(2.5)));
		std::shared_ptr<polygon> p = std::static_pointer_cast<polygon>(a->property);
		std::cout << "polygon point = " << p->point << std::endl;
	}

	return 0;
}

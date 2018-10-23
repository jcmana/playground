#include <iostream>
#include <list>
#include <algorithm>
#include <string>

#include "node_centric.hpp"

template <typename Node>
struct iterator
{
	iterator(Node * start)
	{
		stack.push_back(start);
	}

	void next()
	{
		auto current_node = stack.front();

		for (auto outgoing_edge : current_node->outgoing)
		{
			auto edge_target_node = outgoing_edge->target;
			auto edge_type = outgoing_edge->property;

			// add stack record for next recursion
			stack.push_back(edge_target_node);
		}

		// remove this node from queue
		stack.pop_front();
	}

	Node * get()
	{
		return stack.front();
	}

	bool end()
	{
		return (stack.size() == 0);
	}

	std::list<Node *> stack;
};

template <typename Node>
void flood(Node * start_node)
{
	std::list<Node *> stack;
	stack.push_back(start_node);

	while (stack.size())
	{
		auto current_node = stack.front();

		std::cout << current_node->property << std::endl;

		for (auto outgoing_edge : current_node->outgoing)
		{
			auto edge_target_node = outgoing_edge->target;
			auto edge_type = outgoing_edge->property;

			// add stack record for next recursion
			stack.push_back(edge_target_node);
		}

		// remove this node from queue
		stack.pop_front();
	}
}

int main()
{
	if (true)
	{
		using graph = node_centric_graph<std::string, int>;
		graph g;

		static constexpr int EDGE_DOWN = 1;

		// add nodes
		graph::node * struct_1 = g.create_node("struct_1");
		graph::node * struct_2 = g.create_node("struct_2");
		graph::node * struct_3 = g.create_node("struct_3");
		graph::node * struct_4 = g.create_node("struct_4");

		graph::node * ref_1 = g.create_node("ref_1");
		graph::node * ref_2 = g.create_node("ref_2");
		graph::node * ref_3 = g.create_node("ref_3");
		graph::node * ref_4 = g.create_node("ref_4");

		graph::node * el_1 = g.create_node("el_1");
		graph::node * el_2 = g.create_node("el_2");
		graph::node * el_3 = g.create_node("el_3");
		graph::node * el_4 = g.create_node("el_4");
		graph::node * el_5 = g.create_node("el_5");
		graph::node * el_6 = g.create_node("el_6");

		// add edges
		g.create_edge(struct_1, ref_1, EDGE_DOWN);
		g.create_edge(struct_1, ref_2, EDGE_DOWN);
		g.create_edge(struct_1, el_1, EDGE_DOWN);

		g.create_edge(struct_2, ref_3, EDGE_DOWN);
		g.create_edge(struct_2, el_2, EDGE_DOWN);
		g.create_edge(struct_2, el_3, EDGE_DOWN);

		g.create_edge(struct_3, ref_4, EDGE_DOWN);
		g.create_edge(struct_3, el_4, EDGE_DOWN);

		g.create_edge(struct_4, el_5, EDGE_DOWN);
		g.create_edge(struct_4, el_6, EDGE_DOWN);

		g.create_edge(ref_1, struct_3, EDGE_DOWN);
		g.create_edge(ref_2, struct_2, EDGE_DOWN);
		g.create_edge(ref_4, struct_4, EDGE_DOWN);

		// find node with property
		if (false)
		{
			auto & res = std::find_if(g.nodes.begin(), g.nodes.end(), [](const auto & ptr) -> bool
			{
				return (ptr->property == "node a");
			});

			if (res != g.nodes.end())
			{
				std::cout << "found " << res->get()->property << std::endl;
			}
		}

		// print graph
		if (false) for (const auto & node_it : g.nodes)
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

		// print graph (using flood iterator)
		if (true)
		{
			iterator<graph::node> it(struct_1);
			while (it.end() == false)
			{
				graph::node * node_ptr = it.get();

				std::cout << node_ptr->property << std::endl;

				for (graph::edge * edge_ptr : node_ptr->incoming)
				{
					std::cout << "   <- " << edge_ptr->source->property << std::endl;
				}

				for (graph::edge * edge_ptr : node_ptr->outgoing)
				{
					std::cout << "   -> " << edge_ptr->target->property << std::endl;
				}

				it.next();
			}
		}

		std::cout << std::endl;
	}

	if (false)
	{
		struct element
		{
			explicit element(int t) : type(t) {}
			int type;
		};

		struct polygon :
			element
		{
			explicit polygon(double a, double b) : 
				element(3), 
				point_a(a),
				point_b(b)
			{}
			double point_a;
			double point_b;
		};

		if (true)
		{
			using sp_graph = node_centric_graph<std::shared_ptr<element>, int>;
			sp_graph g;

			auto * a = g.create_node(std::make_shared<polygon>(3.6, 4.4));
			std::shared_ptr<polygon> p = std::static_pointer_cast<polygon>(a->property);
			std::cout << "polygon point = " << p->point_a << std::endl;
		}

		if (true)
		{
			using up_graph = node_centric_graph<std::unique_ptr<element>, int>;
			up_graph g;

			auto * a = g.create_node(new polygon(2.5, 0.5));
			polygon * p = static_cast<polygon *>(a->property.get());
			std::cout << "polygon point = " << p->point_a << std::endl;
		}
	}

	std::getchar(); return 0;
}

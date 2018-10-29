#include <iostream>
#include <list>
#include <algorithm>
#include <string>

#include "../../generic/generic/bidirectional_map.h"
#include "node_centric.hpp"

template <typename Node>
class flood_iterator
{
public:
	flood_iterator()
	{
	}

	flood_iterator(Node * start)
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

public:
	bool operator ==(flood_iterator & other)
	{
		return (stack == other.stack);
	}

	bool operator !=(flood_iterator & other)
	{
		return !(operator ==(other));
	}

	flood_iterator & operator ++()
	{
		next();
		return *this;
	}

	Node * operator ->()
	{
		return get();
	}

	Node operator *()
	{
		return get();
	}

private:
	std::list<Node *> stack;
};

template <typename Node>
class leaf_iterator
{
public:
	leaf_iterator()
	{

	}

	void next()
	{

	}

	Node * get()
	{

	}

	bool end()
	{

	}
};

template <typename Graph>
void leaves(const Graph & g)
{
	auto it = g.nodes.begin();

	while (it != g.nodes.end())
	{
		auto & node = *it;

		if (node->outgoing.size() == 0)
		{
			std::cout << node->property << std::endl;
		}

		it++;
	}
}

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
	if (false)
	{
		using graph = node_centric_graph<std::string, int>;
		graph g;

		static constexpr int EDGE_UP = 1;
		static constexpr int EDGE_DOWN = 2;
		static constexpr int EDGE_ABOVE = 3;
		static constexpr int EDGE_BELOW = 4;

		generic::bidirectional_map<int, std::string> edge_type_map(
		{
			{ EDGE_UP, "up" },
			{ EDGE_DOWN, "down" },
			{ EDGE_ABOVE, "above" },
			{ EDGE_BELOW, "below" },
		});

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
		g.create_edge(ref_3, struct_3, EDGE_DOWN);
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
		if (false)
		{
			flood_iterator<graph::node> it(struct_1);
			while (it.end() == false)
			{
				graph::node * node_ptr = it.get();

				std::cout << node_ptr->property << std::endl;

				/*
				for (graph::edge * edge_ptr : node_ptr->incoming)
				{
					std::cout << "   <- " << edge_ptr->source->property << std::endl;
				}

				for (graph::edge * edge_ptr : node_ptr->outgoing)
				{
					std::cout << "   -> " << edge_ptr->target->property << std::endl;
				}
				*/

				it.next();
			}
		}

		// print graph (using flood iterator with iterator operators)
		if (false)
		{
			std::list<graph::node> stack;

			flood_iterator<graph::node> it(struct_1);
			flood_iterator<graph::node> it_end;

			// TODO: ideal form is:
			// for (graph::node * node_ptr : g.flood(struct_1))
			while (it != it_end)
			{
				std::cout << it->property << std::endl;

				for (graph::edge * edge_ptr : it->incoming)
				{
					std::cout << "   <- " << edge_ptr->source->property << std::endl;
				}

				for (graph::edge * edge_ptr : it->outgoing)
				{
					std::cout << "   -> " << edge_ptr->target->property << std::endl;
				}

				++it;
			}
		}

		// inline flood scene generation
		if (true)
		{
			struct stack_item
			{
				graph::node * curr_model_node;
				graph::node * prev_scene_node;
			};

			std::list<stack_item> stack;

			// initilalize with starting nodes
			stack.push_back({ struct_1, nullptr });

			while (stack.size())
			{
				auto * curr_model_node = stack.front().curr_model_node;
				auto * prev_scene_node = stack.front().prev_scene_node;

				// create current scene node
				auto * curr_scene_node = g.create_node("scene_" + curr_model_node->property);

				// edge to the model node
				if (curr_model_node != nullptr)
				{
					g.create_edge(curr_scene_node, curr_model_node, EDGE_BELOW);
				}

				// edge to the previous (parent) scene node
				if (prev_scene_node != nullptr)
				{
					g.create_edge(prev_scene_node, curr_scene_node, EDGE_DOWN);
				}

				for (auto outgoing_edge : curr_model_node->outgoing)
				{
					auto edge_target_node = outgoing_edge->target;
					auto edge_type = outgoing_edge->property;

					// add stack item for next recursion
					stack.push_back({ edge_target_node, curr_scene_node });
				}

				// remove this node from queue
				stack.pop_front();
			}
		}

		// print graph
		if (true) for (const auto & node_it : g.nodes)
		{
			std::cout << node_it->property << std::endl;

			for (const auto & edge_it : node_it->incoming)
			{
				std::cout << "   <- " << edge_it->source->property << "(" << edge_type_map[edge_it->property] << ")" << std::endl;
			}

			for (const auto & edge_it : node_it->outgoing)
			{
				std::cout << "   -> " << edge_it->target->property << "(" << edge_type_map[edge_it->property] << ")" << std::endl;
			}
		}
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

	if (true)
	{
		// Create the graph object with required properties (std::string node property, int edge property)
		using graph = node_centric_graph<std::string, int>;
		graph g;

		// Create some nodes:
		graph::node * node_a = g.create_node("a");
		graph::node * node_b = g.create_node("b");
		graph::node * node_c = g.create_node("c");

		// Create edges between nodes:
		graph::edge * edge_ab = g.create_edge(node_a, node_b, 23);
		graph::edge * edge_bc = g.create_edge(node_b, node_c, 27);
		graph::edge * edge_cb = g.create_edge(node_c, node_b, 6);

		// Print all nodes:
		std::cout << "Graph nodes:" << std::endl;
		for (auto & node : g.nodes)
		{
			std::cout << node->property << std::endl;
		}

		// Find edge with minimal property:
		auto it_min = std::min_element(g.edges.begin(), g.edges.end(), [](const std::unique_ptr<graph::edge> & a, const std::unique_ptr<graph::edge> & b) -> bool
		{
			return (a->property > b->property);
		});

		// Find node with specific property and create edge from it:
		auto it_find = std::find_if(g.nodes.begin(), g.nodes.end(), [](const auto & node) -> bool
		{
			return ("a" == node->property);
		});

		if (it_find != g.nodes.end())
		{
			g.create_edge(it_find->get(), node_c, 7);
		}

	}

	std::getchar(); return 0;
}

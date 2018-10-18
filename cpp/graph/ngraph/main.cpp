#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <utility>

#include "../../generic/generic/bidirectional_map.h"

#include "node.h"
#include "in_out_graph.h"

static unsigned int counter = 0;

generic::bidirectional_map<in_out_graph::edge_type, std::string> edgetostring 
{
	{
		{ in_out_graph::edge_type::up, "up" },
		{ in_out_graph::edge_type::down, "down" }
	}
};

std::shared_ptr<node> find_node(const std::vector<std::shared_ptr<node>> & nodes, const std::string & name)
{
	for (auto & node : nodes)
	{
		if (node->name() == name)
		{
			return node;
		}
	};

	return std::shared_ptr<node>();
}

/// \brief		Returns first outgoing edge of appropriate edge type from specified node.
std::shared_ptr<node> follow_outgoing_edge(in_out_graph & g, const std::shared_ptr<node> & n, in_out_graph::edge_type type = in_out_graph::edge_type::typeless)
{
	std::shared_ptr<node> result;

	auto & outgoing_edges = g.outgoing(n);

	for (auto & outgoing_edge : outgoing_edges)
	{
		if (outgoing_edge.second == type)
		{
			result = outgoing_edge.first;
			break;
		}
	}

	return result;
}

std::set<std::shared_ptr<node>> follow_outgoing_edges(in_out_graph & g, const std::shared_ptr<node> & n, in_out_graph::edge_type type = in_out_graph::edge_type::typeless)
{
	std::set<std::shared_ptr<node>> result;

	auto & outgoing_edges = g.outgoing(n);

	for (auto & outgoing_edge : outgoing_edges)
	{
		if (outgoing_edge.second == type)
		{
			result.insert(outgoing_edge.first);
		}
	}

	return result;
}

using predicate = bool (*)(const std::shared_ptr<node> & n, in_out_graph::edge_type t);

std::set<std::shared_ptr<node>> follow_outgoing_edges(in_out_graph & g, const std::shared_ptr<node> & n, predicate p)
{
	std::set<std::shared_ptr<node>> result;

	auto & outgoing_edges = g.outgoing(n);

	for (auto & outgoing_edge : outgoing_edges)
	{
		if (p(outgoing_edge.first, outgoing_edge.second))
		{
			result.insert(outgoing_edge.first);
		}
	}

	return result;
}

int test_plain_graph()
{
	std::vector<std::shared_ptr<node>> model_nodes
	{
		std::make_shared<node>("struct_1"),
		std::make_shared<node>("struct_2"),
		std::make_shared<node>("struct_3"),
		std::make_shared<node>("struct_4"),

		std::make_shared<node>("ref_1"),
		std::make_shared<node>("ref_2"),
		std::make_shared<node>("ref_3"),
		std::make_shared<node>("ref_4"),

		std::make_shared<node>("el_1"),
		std::make_shared<node>("el_2"),
		std::make_shared<node>("el_3"),
		std::make_shared<node>("el_4"),
		std::make_shared<node>("el_5"),
		std::make_shared<node>("el_6")
	};

	// 1:N relation
	std::map<std::shared_ptr<node>, std::set<std::shared_ptr<node>>> model_edges
	{
		{ find_node(model_nodes, "struct_1"),{ find_node(model_nodes, "ref_1"), find_node(model_nodes, "ref_2"), find_node(model_nodes, "el_1") } },
		{ find_node(model_nodes, "struct_2"),{ find_node(model_nodes, "ref_3"), find_node(model_nodes, "el_2"), find_node(model_nodes, "el_3") } },
		{ find_node(model_nodes, "struct_3"),{ find_node(model_nodes, "el_4"), find_node(model_nodes, "ref_4") } },
		{ find_node(model_nodes, "struct_4"),{ find_node(model_nodes, "el_5"), find_node(model_nodes, "el_6") } },

		{ find_node(model_nodes, "ref_1"),{ find_node(model_nodes, "struct_3") } },
		{ find_node(model_nodes, "ref_2"),{ find_node(model_nodes, "struct_2") } },
		{ find_node(model_nodes, "ref_3"),{ find_node(model_nodes, "struct_3") } },
		{ find_node(model_nodes, "ref_4"),{ find_node(model_nodes, "struct_4") } }
	};

	// 1:1 relation
	std::map<std::shared_ptr<node>, std::shared_ptr<node>> scene_to_model_edges;

	// 1:N relation
	std::map<std::shared_ptr<node>, std::set<std::shared_ptr<node>>> scene_edges;

	std::list<std::shared_ptr<node>> unchecked;

	for (auto & model_node : model_nodes)
	{
		auto scene_node = std::make_shared<node>("scene_" + model_node->name() + "(" + std::to_string(counter++) + ")");

		// create edge from scene to model
		scene_to_model_edges.insert(std::make_pair(scene_node, model_node));

		// flag node as unchecked
		unchecked.push_back(scene_node);
	}

	std::shared_ptr<node> unchecked_scene_node;
	std::shared_ptr<node> model_node;

	while (unchecked.size())
	{
		unchecked_scene_node = unchecked.front();

		// get related model node
		model_node = scene_to_model_edges[unchecked_scene_node];

		std::cout << "unchecked list: front -> ";
		for (auto & u : unchecked)
		{
			std::cout << u->name() << " ";
		}
		std::cout << "<- back" << std::endl;
		std::cout << "checking: " << unchecked_scene_node->name() << " -> " << model_node->name() << std::endl;

		// check related model node outgoing edges
		for (auto & model_node_edge : model_edges[model_node])
		{
			// is this edge already made?
			bool alreadyThere = false;
			for (auto & existing_node_edge : scene_edges[unchecked_scene_node])
			{
				if (model_node_edge == scene_to_model_edges[existing_node_edge])
				{
					alreadyThere = true;
				}
			}

			if (alreadyThere)
			{
				std::cout << "   edge already exists: " << model_node_edge->name() << std::endl;
				continue;
			}

			std::shared_ptr<node> desired_scene_node;

			// try to find unchecked scene node with desired model node relation
			for (auto & unchecked_scene_node_find : unchecked)
			{
				if (scene_to_model_edges[unchecked_scene_node_find] == model_node_edge)
				{
					desired_scene_node = unchecked_scene_node_find;
				}
			}

			// desired scene node can't have incoming edges
			for (auto & scene_edge : scene_edges)
			{
				for (auto & scene_node_edge : scene_edge.second)
				{
					if (scene_node_edge == desired_scene_node)
					{
						desired_scene_node.reset();
					}
				}
			}

			if (desired_scene_node)
			{
				std::cout << "   adding edge to: " << desired_scene_node->name() << std::endl;

				// create edge from scene node to related scene node
				scene_edges[unchecked_scene_node].insert(desired_scene_node);

				continue;
			}

			// no unchecked scene node fits, create new one
			desired_scene_node = std::make_shared<node>("scene_" + model_node_edge->name() + "(" + std::to_string(counter++) + ")");

			std::cout << "   creating node: " << desired_scene_node->name() << std::endl;

			// create edge from scene node to related scene node
			std::cout << "   adding edge to: " << desired_scene_node->name() << std::endl;

			scene_edges[unchecked_scene_node].insert(desired_scene_node);

			// create edge from scene to model
			scene_to_model_edges.insert(std::make_pair(desired_scene_node, model_node_edge));

			// append created node to unchecked
			unchecked.push_back(desired_scene_node);
		}

		// check related model node incoming edges
		// TODO: this should probably be replaced by list of incoming edges

		bool incoming_edges_expected = false;
		bool checked = false;

		for (auto & model_edge : model_edges)
		{
			for (auto & model_edge_outgoing : model_edge.second)
			{
				if (model_edge_outgoing == model_node)
				{
					incoming_edges_expected = true;
				}
			}

			if (incoming_edges_expected)
			{
				break;
			}
		}

		if (incoming_edges_expected)
		{
			for (auto & scene_edge : scene_edges)
			{
				for (auto & scene_node_edge : scene_edge.second)
				{
					if (scene_node_edge == unchecked_scene_node)
					{
						std::cout << "   found expected incoming from: " << scene_edge.first->name() << std::endl;
						checked = true;
					}
				}
			}

			if (checked == false)
			{
				std::cout << "   expected incoming not found" << std::endl;
			}
		}
		else
		{
			std::cout << "   no expected incoming" << std::endl;
			checked = true;

		}

		if (checked)
		{
			std::cout << "   removing from unchecked " << std::endl;
			unchecked.pop_front();
		}
		else
		{
			std::cout << "   moving to the end of list" << std::endl;
			unchecked.pop_front();
			unchecked.push_back(unchecked_scene_node);
		}

		std::cout << std::endl;
	}

	return 0;
}

int test_in_out_graph()
{
	// ================================================================================
	//	build the graph
	// ================================================================================
	in_out_graph g;
	{
		auto struct_1 = std::make_shared<node>("struct_1");
		auto struct_2 = std::make_shared<node>("struct_2");
		auto struct_3 = std::make_shared<node>("struct_3");
		auto struct_4 = std::make_shared<node>("struct_4");
		auto struct_5 = std::make_shared<node>("struct_5");

		auto ref_1 = std::make_shared<node>("ref_1");
		auto ref_2 = std::make_shared<node>("ref_2");
		auto ref_3 = std::make_shared<node>("ref_3");
		auto ref_4 = std::make_shared<node>("ref_4");

		auto el_1 = std::make_shared<node>("el_1");
		auto el_2 = std::make_shared<node>("el_2");
		auto el_3 = std::make_shared<node>("el_3");
		auto el_4 = std::make_shared<node>("el_4");
		auto el_5 = std::make_shared<node>("el_5");
		auto el_6 = std::make_shared<node>("el_6");
		auto el_7 = std::make_shared<node>("el_7");

		g.add_node(struct_1);
		g.add_node(struct_2);
		g.add_node(struct_3);
		g.add_node(struct_4);
		g.add_node(struct_5);

		g.add_node(ref_1);
		g.add_node(ref_2);
		g.add_node(ref_3);
		g.add_node(ref_4);

		g.add_node(el_1);
		g.add_node(el_2);
		g.add_node(el_3);
		g.add_node(el_4);
		g.add_node(el_5);
		g.add_node(el_6);
		g.add_node(el_7);

		g.add_edge(struct_1, ref_1, in_out_graph::edge_type::down);
		g.add_edge(struct_1, ref_2, in_out_graph::edge_type::down);
		g.add_edge(struct_1, el_1, in_out_graph::edge_type::down);

		g.add_edge(struct_2, ref_3, in_out_graph::edge_type::down);
		g.add_edge(struct_2, el_2, in_out_graph::edge_type::down);
		g.add_edge(struct_2, el_3, in_out_graph::edge_type::down);

		g.add_edge(struct_3, el_4, in_out_graph::edge_type::down);
		g.add_edge(struct_3, ref_4, in_out_graph::edge_type::down);

		g.add_edge(struct_4, el_5, in_out_graph::edge_type::down);
		g.add_edge(struct_4, el_6, in_out_graph::edge_type::down);

		g.add_edge(struct_5, el_7, in_out_graph::edge_type::down);

		g.add_edge(ref_1, struct_3, in_out_graph::edge_type::down);
		g.add_edge(ref_2, struct_2, in_out_graph::edge_type::down);
		g.add_edge(ref_3, struct_3, in_out_graph::edge_type::down);
		g.add_edge(ref_4, struct_4, in_out_graph::edge_type::down);
	}

	// ================================================================================
	//	print the graph
	// ================================================================================
	if (false) for (auto it_node = g.begin_nodes(); it_node != g.end_nodes(); it_node++)
	{
		const std::shared_ptr<node> & current_node = *it_node;

		std::cout << current_node->name() << std::endl;

		for (auto & incoming_edge : g.incoming(current_node))
		{
			auto & node = incoming_edge.first;
			auto & type = incoming_edge.second;

			std::cout << "   <- " << node->name() << " (" << edgetostring[type] << ")" << std::endl;
		}

		for (auto & outgoing_edge : g.outgoing(current_node))
		{
			auto & node = outgoing_edge.first;
			auto & type = outgoing_edge.second;

			std::cout << "   -> " << node->name() << " (" << edgetostring[type] << ")" << std::endl;
		}

		std::cout << std::endl;
	}

	// ================================================================================
	//	find top-level nodes
	// ================================================================================
	std::set<std::shared_ptr<node>> toplevel_model_nodes;
	for (auto it_node = g.begin_nodes(); it_node != g.end_nodes(); it_node++)
	{
		auto & current_node = *it_node;

		if (g.incoming(current_node).size() == 0)
		{
			toplevel_model_nodes.insert(current_node);
		}
	}


	// ================================================================================
	//	unchecked algorithm (unfinished)
	// ================================================================================
	if (false)
	{
		std::list<std::shared_ptr<node>> unchecked;

		for (auto toplevel_node : toplevel_model_nodes)
		{
			// create new scene node
			auto scene_toplevel_node = std::make_shared<node>("scene_struct_1");

			// add the node and create an edge to the model node
			g.add_node(scene_toplevel_node);
			g.add_edge(scene_toplevel_node, toplevel_node, in_out_graph::edge_type::below);

			// queue as unchecked
			unchecked.push_back(scene_toplevel_node);
		}

		std::shared_ptr<node> current_scene_node;
		std::shared_ptr<node> current_model_node;

		while (unchecked.size())
		{
			// retrieve current scene node to check
			current_scene_node = unchecked.front();

			// retrieve related model scene node
			for (auto & current_scene_node_outgoing : g.outgoing(current_scene_node))
			{
				if (current_scene_node_outgoing.second == in_out_graph::edge_type::below)
				{
					current_model_node = current_scene_node_outgoing.first;
				}
			}

			std::cout << current_scene_node->name() << " -> " << current_model_node->name() << std::endl;

			// scene node completeness flags
			bool missing_incoming_edges = true;
			bool missing_outgoing_edges = true;

			// compare scene node and model node incoming edges
			auto & current_scene_node_incoming = g.incoming(current_scene_node);
			auto & current_model_node_incoming = g.incoming(current_model_node);

			if (current_model_node_incoming.size() > 0)
			{
				for (auto & edge : current_scene_node_incoming)
				{
					if (edge.second == in_out_graph::edge_type::down)
					{
						// node is not complete => missing incoming edge
						missing_incoming_edges = false;
					}
				}
			}

			// compare scene node and model node outgoing edges
			auto & current_scene_node_outgoing = g.outgoing(current_scene_node);
			auto & current_model_node_outgoing = g.outgoing(current_model_node);

			if (current_model_node_outgoing.size() > 0)
			{
				for (auto & edge : current_scene_node_outgoing)
				{
					if (edge.second == in_out_graph::edge_type::down)
					{
						// node is not complete => missing incoming edge
						missing_outgoing_edges = false;
					}
				}
			}


			// remove from unchecked if 
			if (missing_incoming_edges == false && missing_outgoing_edges == false)
			{
				unchecked.pop_front();
			}
		}
	}

	// ================================================================================
	//	top-level node stack-recursion algorithm
	// ================================================================================
	if (false)
	{
		std::list<std::shared_ptr<node>> scene_node_queue;

		// create scene node for each toplevel model node
		for (auto & toplevel_model_node : toplevel_model_nodes)
		{
			// create new scene node
			auto toplevel_scene_node = std::make_shared<node>("scene_" + toplevel_model_node->name());

			// add node to the graph
			g.add_node(toplevel_scene_node);

			// add the node and create an edge to the model node
			g.add_node(toplevel_scene_node);
			g.add_edge(toplevel_scene_node, toplevel_model_node, in_out_graph::edge_type::below);

			// queue for recursion
			scene_node_queue.push_back(toplevel_scene_node);
		}

		while (scene_node_queue.size())
		{
			auto & source_scene_node = scene_node_queue.front();
			auto & source_model_node = follow_outgoing_edge(g, source_scene_node, in_out_graph::edge_type::below);

			for (auto & target_model_node : follow_outgoing_edges(g, source_model_node, in_out_graph::edge_type::down))
			{
				// generate scene node
				auto target_scene_node = std::make_shared<node>("scene_" + target_model_node->name());

				// add node to the graph
				g.add_node(target_scene_node);

				// create scene node -> model node edge
				g.add_edge(target_scene_node, target_model_node, in_out_graph::edge_type::below);

				// create source scene node -> target scene node
				g.add_edge(source_scene_node, target_scene_node, in_out_graph::edge_type::down);

				// add target scene node into scene_node_queue for next recursion
				scene_node_queue.push_back(target_scene_node);
			}

			// remove this node from queue
			scene_node_queue.pop_front();
		}
	}

	// ================================================================================
	//	print the graph (outgoing(down) edges only)
	// ================================================================================
	if (false) for (auto it_node = g.begin_nodes(); it_node != g.end_nodes(); it_node++)
	{
		auto & current_node = *it_node;

		std::cout << current_node->name() << std::endl;

		auto & outgoing_nodes = follow_outgoing_edges(g, current_node, in_out_graph::edge_type::down);

		for (auto & outgoing_node : outgoing_nodes)
		{
			std::cout << "   -> " << outgoing_node->name() << std::endl;
		}
	}

	// ================================================================================
	//	leaf identity algorithm
	// ================================================================================
	if (false) for (auto it_node = g.begin_nodes(); it_node != g.end_nodes(); it_node++)
	{
		auto & current_node = *it_node;
	
		// check if node is a scene node (prefix "scene_")
		// TODO: this obviously needs to be replace by adding and checking node type
		// in full element nodes implementation
		std::string scene_node_prefix("scene_");
		if (scene_node_prefix.compare(0, scene_node_prefix.size(), current_node->name(), 0, scene_node_prefix.size()) != 0)
		{
			continue;
		}

		// check if node is a leaf node (leaf nodes have no outgoing edges)
		auto & outgoing_nodes = follow_outgoing_edges(g, current_node, in_out_graph::edge_type::down);
		if (outgoing_nodes.size() > 0)
		{
			continue;
		}

		// create view node
		auto view_node = std::make_shared<node>("view_" + current_node->name());

		// add the node and create an edge to the scene node
		g.add_node(view_node);
		g.add_edge(view_node, current_node, in_out_graph::edge_type::below);
	}

	// ================================================================================
	//	print the graph (outgoing(below) edges only)
	// ================================================================================
	if (false) for (auto it_node = g.begin_nodes(); it_node != g.end_nodes(); it_node++)
	{
		auto & current_node = *it_node;

		std::cout << current_node->name() << std::endl;

		auto & outgoing_nodes = follow_outgoing_edges(g, current_node, in_out_graph::edge_type::below);

		for (auto & outgoing_node : outgoing_nodes)
		{
			std::cout << "   -> " << outgoing_node->name() << std::endl;
		}
	}

	// ================================================================================
	//	recursive graph traverse
	// ================================================================================
	if (true)
	{
		// node stack (initialized with all top-level nodes = recursion starting point)
		std::list<std::shared_ptr<node>> node_stack(toplevel_model_nodes.begin(), toplevel_model_nodes.end());

		// edge type to follow during recursion
		in_out_graph::edge_type type = in_out_graph::edge_type::down;

		while (node_stack.size())
		{
			auto & current_node = node_stack.front();

			std::cout << current_node->name() << std::endl;

			for (auto & outgoing_edge : g.outgoing(current_node))
			{
				if (outgoing_edge.second == type)
				{
					// add the node for next recursion
					node_stack.push_back(outgoing_edge.first);
				}
			}

			// remove this node from queue
			node_stack.pop_front();
		}
	}

	return 0;
}

int main()
{
	if (false)
		return test_plain_graph();

	if (true)
		return test_in_out_graph();

	return 0;
}

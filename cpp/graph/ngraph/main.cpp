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

int test_in_out_graph()
{
	in_out_graph g;

	auto struct_1 = std::make_shared<node>("struct_1");
	auto struct_2 = std::make_shared<node>("struct_2");
	auto struct_3 = std::make_shared<node>("struct_3");
	auto struct_4 = std::make_shared<node>("struct_4");

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

	g.add_node(struct_1);
	g.add_node(struct_2);
	g.add_node(struct_3);
	g.add_node(struct_4);

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

	g.add_edge(ref_1, struct_3, in_out_graph::edge_type::down);
	g.add_edge(ref_2, struct_2, in_out_graph::edge_type::down);
	g.add_edge(ref_3, struct_3, in_out_graph::edge_type::down);
	g.add_edge(ref_4, struct_4, in_out_graph::edge_type::down);

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

	std::list<std::shared_ptr<node>> unchecked;

	{
		auto scene_struct_1 = std::make_shared<node>("scene_struct_1");
		g.add_node(scene_struct_1);
		g.add_edge(scene_struct_1, struct_1, in_out_graph::edge_type::below);
	}

	return 0;
}

int main()
{
	return test_in_out_graph();

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
		{ find_node(model_nodes, "struct_1"), { find_node(model_nodes, "ref_1"), find_node(model_nodes, "ref_2"), find_node(model_nodes, "el_1") } },
		{ find_node(model_nodes, "struct_2"), { find_node(model_nodes, "ref_3"), find_node(model_nodes, "el_2"), find_node(model_nodes, "el_3") } },
		{ find_node(model_nodes, "struct_3"), { find_node(model_nodes, "el_4"), find_node(model_nodes, "ref_4") } },
		{ find_node(model_nodes, "struct_4"), { find_node(model_nodes, "el_5"), find_node(model_nodes, "el_6") } },

		{ find_node(model_nodes, "ref_1"), { find_node(model_nodes, "struct_3") } },
		{ find_node(model_nodes, "ref_2"), { find_node(model_nodes, "struct_2") } },
		{ find_node(model_nodes, "ref_3"), { find_node(model_nodes, "struct_3") } },
		{ find_node(model_nodes, "ref_4"), { find_node(model_nodes, "struct_4") } }
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
				std::cout << "   expected incoming not found" <<  std::endl;
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

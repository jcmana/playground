#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <utility>

class node
{
public:
	node(std::string name = "node") :
		m_name(name)
	{
	}

	std::string name() const
	{
		return m_name;
	}

private:
	std::string m_name;
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

int main()
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

	// 1:1 relation
	std::map<std::shared_ptr<node>, std::shared_ptr<node>> scene_edges;

	std::list<std::shared_ptr<node>> unchecked;

	for (auto & model_node : model_nodes)
	{
		auto scene_node = std::make_shared<node>("scene_" + model_node->name());

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

		std::cout << "checking: " << unchecked_scene_node->name() << " -> " << model_node->name() << std::endl;

		// get related model node edges
		for (auto & model_node_edge : model_edges[model_node])
		{
			std::shared_ptr<node> desired_scene_node;

			// try to find unchecked scene node with desired model node relation
			for (auto & unchecked_scene_node_find : unchecked)
			{
				if (scene_to_model_edges[unchecked_scene_node_find] == model_node_edge)
				{
					desired_scene_node = unchecked_scene_node_find;
				}
			}

			if (desired_scene_node)
			{
				std::cout << "   adding edge to: " << desired_scene_node->name() << std::endl;

				// create edge from scene node to related scene node
				scene_edges.insert(std::make_pair(unchecked_scene_node, desired_scene_node));

				continue;
			}

			// no unchecked scene node fits, create new one
			desired_scene_node = std::make_shared<node>("scene_" + model_node_edge->name());

			std::cout << "   creating node: " << desired_scene_node->name() << std::endl;

			// create edge from scene node to related scene node
			std::cout << "   adding edge to: " << desired_scene_node->name() << std::endl;

			scene_edges.insert(std::make_pair(unchecked_scene_node, desired_scene_node));

			// create edge from scene to model
			scene_to_model_edges.insert(std::make_pair(desired_scene_node, model_node_edge));

			// append created node to unchecked
			unchecked.push_back(desired_scene_node);
		}

		unchecked.pop_front();
	}

	std::getchar();  return 0;
}

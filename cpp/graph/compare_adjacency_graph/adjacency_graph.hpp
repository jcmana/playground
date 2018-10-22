#pragma once

#include <memory>
#include <set>
#include <map>
#include <vector>
#include <utility>

//template<typename Node, typename Edge>
using Node = int;
using Edge = int;
class adjacency_graph
{
public:
	struct edge_descriptor
	{
		const Node & source;
		const Node & target;
		const Edge & property;
	};

public:
	void add_edge(const Node & source, const Node & target, const Edge & property)
	{
		std::pair<std::set<Node>::iterator, bool> result;
	
		result = m_nodes.insert(source);
		const Node & inserted_source = *(result.first);

		result = m_nodes.insert(target);
		const Node & inserted_target = *(result.first);
		
		result = m_edges.insert(property);
		const Edge & inserted_property = *(result.first);

		edge_descriptor e = { inserted_source, inserted_target, inserted_property };

		m_incoming[target].push_back(e);
		m_outgoing[source].push_back(e);
	}

private:
	std::set<Node> m_nodes;
	std::set<Edge> m_edges;

	std::map<Node, std::vector<edge_descriptor>> m_incoming;
	std::map<Node, std::vector<edge_descriptor>> m_outgoing;
};

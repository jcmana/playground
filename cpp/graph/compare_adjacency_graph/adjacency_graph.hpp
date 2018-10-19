#pragma once

#include <memory>
#include <set>
#include <map>
#include <utility>

//template<typename Node, typename Edge>
using Node = int;
using Edge = int;
class adjacency_graph
{
public:
	struct edge_descriptor
	{
		Node & source;
		Node & target;
		Edge & property;
	};

public:
	void add_edge(const Node & source, const Node & target, const Edge & property)
	{
		std::pair<std::set<Node>::iterator, bool> result;
		result = m_nodes.emplace(source);

		Node & src = result.first;

		//edge_descriptor e = { *result.first, target, property };
		//m_incoming[source]
	}

private:
	std::set<Node> m_nodes;
	std::set<Edge> m_edges;

	std::map<Node *, std::set<edge_descriptor>> m_incoming;
	std::map<Node *, std::set<edge_descriptor>> m_outgoing;
};

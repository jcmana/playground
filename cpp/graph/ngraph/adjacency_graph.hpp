#pragma once

#include <memory>
#include <set>
#include <map>
#include <utility>

#include "adjacency_graph_node.hpp"
#include "adjacency_graph_edge.hpp"

//template<typename NodeProperty, typename EdgeProperty>

using NodeProperty = int;
using EdgeProperty = unsigned int;

class adjacency_graph
{
public:
	using node_owner_type = adjacency_graph_node<NodeProperty>;
	using edge_owner_type = adjacency_graph_edge<EdgeProperty>;
	using edge_type = std::pair<node_owner_type *, edge_owner_type *>;

public:
	// modifiers:

	node_owner_type * create_node(const NodeProperty & property)
	{
		return create_node_owner(property);
	}

	edge_owner_type * create_edge(const EdgeProperty & property)
	{
		return create_edge_owner(property);
	}

	void add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge)
	{
		edge_type edge_pair = std::make_pair(source, edge);
		m_edges_outgoing[source].insert(edge_pair);
		m_edges_incoming[target].insert(edge_pair);
	}

	void make_edge(const NodeProperty & source, const NodeProperty & target, const EdgeProperty & edge)
	{
		auto * source_owner = create_node(source);
		auto * target_owner = create_node(target);
		auto * edge_owner = create_edge(edge);

		add_edge(source_owner, target_owner, edge_owner);
	}


	// enumerations:


private:
	node_owner_type * create_node_owner(const NodeProperty & property)
	{
		auto p_node_owner = new node_owner_type(property);
		m_node_owners.emplace(std::unique_ptr<node_owner_type>(p_node_owner));
		return p_node_owner;
	}

	edge_owner_type * create_edge_owner(const EdgeProperty & property)
	{
		auto p_edge_owner = new edge_owner_type(property);
		m_edge_owners.emplace(std::unique_ptr<edge_owner_type>(p_edge_owner));
		return p_edge_owner;
	}

private:
	std::set<std::unique_ptr<node_owner_type>> m_node_owners;
	std::set<std::unique_ptr<edge_owner_type>> m_edge_owners;

	std::map<node_owner_type *, std::set<edge_type>> m_edges_incoming;
	std::map<node_owner_type *, std::set<edge_type>> m_edges_outgoing;
};

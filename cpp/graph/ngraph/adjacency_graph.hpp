#pragma once

#include <memory>
#include <set>
#include <map>
#include <utility>

#include "adjacency_graph_node.hpp"
#include "adjacency_graph_edge.hpp"

/// \brief		Graph specialization supporting:
///					- fast incoming/outgoing edges access
///					- directed edges
///					- custom node property
///					- custom edge property

//template<typename NodeProperty, typename EdgeProperty>

using NodeProperty = std::string;
using EdgeProperty = int;

class adjacency_graph
{
public:
	/// \brief		Node property owning type.
	using node_owner_type = adjacency_graph_node<NodeProperty>;
	/// \brief		Edge property owning type.
	using edge_owner_type = adjacency_graph_edge<EdgeProperty>;
	/// \brief		Edge owning type.
	using edge_type = std::pair<node_owner_type *, edge_owner_type *>;

public:
	// modifiers:


	/// \brief		Creates node property owner.
	node_owner_type * create_node(const NodeProperty & property)
	{
		return create_node_owner(property);
	}

	/// \brief		Creates edge property owner.
	edge_owner_type * create_edge(const EdgeProperty & property)
	{
		return create_edge_owner(property);
	}

	/// \brief		Adds directed edge from source to target with edge property.
	void add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge)
	{
		m_edges_outgoing[source].emplace(std::make_pair(target, edge));
		m_edges_incoming[target].emplace(std::make_pair(source, edge));
	}

	// enumerations:

	/// \brief		Enumerate all outgoing edges from given node.
	auto & outgoing(node_owner_type * source)
	{
		return m_edges_outgoing[source];
	}


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

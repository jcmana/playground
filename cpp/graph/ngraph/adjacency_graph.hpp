#pragma once

#include <memory>
#include <set>
#include <map>
#include <utility>

#include "adjacency_graph_types.h"

class adjacency_graph_enumerator;

/// \brief		Graph specialization for edge traversing and adjacenct enumeration.
/// \desc		Graph specialization supporting:
///					- fast incoming/outgoing edges access
///					- directed edges
///					- custom node property
///					- custom edge property

using NodeProperty = std::string;
using EdgeProperty = int;
//template<typename NodeProperty, typename EdgeProperty>
class adjacency_graph
{
private:
	using types = adjacency_graph_types<NodeProperty, EdgeProperty>;

public:
	/// \brief		Node property owning type.
	using node_owner_type = types::node_owner_type;
	/// \brief		Edge property owning type.
	using edge_owner_type = types::edge_owner_type;
	/// \brief		Edge owning type.
	using edge_type = types::edge_type;

/*
	/// \brief		Node property owning type.
	using node_owner_type = adjacency_graph_node<NodeProperty>;
	/// \brief		Edge property owning type.
	using edge_owner_type = adjacency_graph_edge<EdgeProperty>;
	/// \brief		Edge owning type.
	using edge_type = std::pair<node_owner_type *, edge_owner_type *>;
*/

public:
	// modifiers:

	/// \brief		Creates node property owner.
	node_owner_type * create_node(const NodeProperty & property);

	/// \brief		Creates edge property owner.
	edge_owner_type * create_edge(const EdgeProperty & property);

	/// \brief		Adds directed edge from source to target with edge property.
	void add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge);

	// enumerations:

	/// \brief		Enumerate all outgoing edges from given node.
	auto & outgoing(node_owner_type * source);

	adjacency_graph_enumerator outgoing(node_owner_type * source, const EdgeProperty & property);

private:
	node_owner_type * create_node_owner(const NodeProperty & property);

	edge_owner_type * create_edge_owner(const EdgeProperty & property);

private:
	std::set<std::unique_ptr<node_owner_type>> m_node_owners;
	std::set<std::unique_ptr<edge_owner_type>> m_edge_owners;

	std::map<node_owner_type *, std::set<edge_type>> m_edges_incoming;
	std::map<node_owner_type *, std::set<edge_type>> m_edges_outgoing;

private:
	friend class adjacency_graph_enumerator;
	friend class adjacency_graph_enumerator_iterator;
};


#include "adjacency_graph_enumerator.hpp"

adjacency_graph::node_owner_type * adjacency_graph::create_node(const NodeProperty & property)
{
	return create_node_owner(property);
}

adjacency_graph::edge_owner_type * adjacency_graph::create_edge(const EdgeProperty & property)
{
	return create_edge_owner(property);
}

void adjacency_graph::add_edge(node_owner_type * source, node_owner_type * target, edge_owner_type * edge)
{
	m_edges_outgoing[source].emplace(std::make_pair(target, edge));
	m_edges_incoming[target].emplace(std::make_pair(source, edge));
}

auto & adjacency_graph::outgoing(node_owner_type * source)
{
	return m_edges_outgoing[source];
}

adjacency_graph_enumerator adjacency_graph::outgoing(node_owner_type * source, const EdgeProperty & property)
{
	return adjacency_graph_enumerator(&m_edges_outgoing, source, property);
}

adjacency_graph::node_owner_type * adjacency_graph::create_node_owner(const NodeProperty & property)
{
	auto p_node_owner = new node_owner_type(property);
	m_node_owners.emplace(std::unique_ptr<node_owner_type>(p_node_owner));
	return p_node_owner;
}

adjacency_graph::edge_owner_type * adjacency_graph::create_edge_owner(const EdgeProperty & property)
{
	auto p_edge_owner = new edge_owner_type(property);
	m_edge_owners.emplace(std::unique_ptr<edge_owner_type>(p_edge_owner));
	return p_edge_owner;
}

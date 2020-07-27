#pragma once

#include <vector>
#include <utility>

namespace containers {
namespace graph {

template <typename N, typename E>
class node_centric
{
public:
	/// \brief		Base container type for storing graph elements.
	template<typename I>
	using container = std::vector<I>;

public:
	/// \brief		Node identification.
	struct node
	{
		std::size_t offset;
	};

	/// \brief		Edge identification.
	struct edge
	{
		std::size_t offset;
	};

	/// \brief		Storage for the node property and edges.
	struct node_storage
	{
		container<edge> outgoing;
		container<edge> incoming;

		N property;
	};

	/// \brief		Storage for the edge property and connected nodes.
	struct edge_storage
	{
		node source;
		node target;

		E property;
	};

	/// \brief		Create a node with property.
	template <typename ... A>
	node create_node(A && ... node_property_args)
	{
		node_storage ns = {{}, {}, node_property_args ...};
		m_nodes.emplace_back(std::move(ns));
		node n = {m_nodes.size() - 1};

		return n;
	}

	/// \brief		Create an edge with property between nodes.
	template <typename ... A>
	edge create_edge(node source_node, node target_node, A && ... edge_property_args)
	{
		edge_storage es = {source_node.offset, target_node.offset, edge_property_args ...};
		m_edges.emplace_back(std::move(es));
		edge e = {m_edges.size() - 1};

		m_nodes[source_node.offset].outgoing.push_back({e.offset});
		m_nodes[target_node.offset].incoming.push_back({e.offset});

		return e;
	}

	/// \brief		Access node's data.
	const node_storage & operator [](node n) const
	{
		return m_nodes[n.offset];
	}

	/// \brief		Access edge's data.
	const edge_storage & operator [](edge e) const
	{
		return m_edges[e.offset];
	}

	const auto & nodes() const
	{
		return m_nodes;
	}

	const auto & edges() const
	{
		return m_edges;
	}

private:
	container<node_storage> m_nodes;
	container<edge_storage> m_edges;
};

} // namespace graph
} // namespace containers

#pragma once

#include <vector>
#include <utility>

namespace containers::graph
{

/// \brief      Adjacency list graph.
template <typename N, typename E, template<typename> typename C = std::vector>
class adjacency
{
public:
	/// \brief		Base container type for storing graph elements.
	template<typename I>
	using container = C<I>;

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

	/// \brief		Adds a `node` with `node_property`.
	node add_node(N node_property)
	{
		node_storage ns = {{}, {}, std::move(node_property)};
		m_nodes.emplace_back(std::move(ns));
        node n = {m_nodes.size() - 1};
		
        return n;
	}

	/// \brief		Adds an `edge` with `edge_property` from `source_node` to `target_node`.
	edge add_edge(node source_node, node target_node, E edge_property)
	{
		edge_storage es = {source_node.offset, target_node.offset, std::move(edge_property)};
		m_edges.emplace_back(std::move(es));
		edge e = {m_edges.size() - 1};

		m_nodes[source_node.offset].outgoing.push_back(e);
		m_nodes[target_node.offset].incoming.push_back(e);

		return e;
	}

    /// \brief		Access node's data with boundary check.
    const auto & at(node n) const
    {
        if (n >= m_nodes.size())
        {
            throw std::out_of_range("node out of bounds");
        }

        return m_nodes[n.offset];
    }

    /// \brief		Access edge's data.
    const auto & at(edge e) const
    {
        if (e >= m_edges.size())
        {
            throw std::out_of_range("edge out of bounds");
        }

        return m_edges[e.offset];
    }

	/// \brief		Access node's data.
	const auto & operator [](node n) const
	{
		return m_nodes[n.offset];
	}

	/// \brief		Access edge's data.
	const auto & operator [](edge e) const
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

} // namespace containers::graph

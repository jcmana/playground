#pragma once

#include <memory>
#include <set>

template <typename N, typename E>
class node_centric_graph
{
public:
	class nc_node;
	class nc_edge;

	struct nc_node
	{
		std::set<nc_edge *> outgoing;
		std::set<nc_edge *> incoming;
		N property;
	};

	struct nc_edge
	{
		nc_node * source;
		nc_node * target;
		E property;
	};

public:
	nc_node * create_node(N node_property)
	{
		auto * node_ptr = new nc_node { {},{}, node_property };
		m_nodes.insert(std::unique_ptr<nc_node>(node_ptr));
		return node_ptr;
	}

	nc_edge * create_edge(nc_node * source, nc_node * target, E edge_property)
	{
		auto * edge_ptr = new nc_edge { source, target, edge_property };
		m_edges.insert(std::unique_ptr<nc_edge>(edge_ptr));
		source->outgoing.insert(edge_ptr);
		target->incoming.insert(edge_ptr);
		return edge_ptr;
	}

public:
	std::set<std::unique_ptr<nc_node>> m_nodes;
	std::set<std::unique_ptr<nc_edge>> m_edges;
};

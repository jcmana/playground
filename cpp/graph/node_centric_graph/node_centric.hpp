#pragma once

#include <memory>
#include <set>

template <typename N, typename E>
struct node_centric_graph
{
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

	nc_node * create_node(N node_property);
	nc_edge * create_edge(nc_node * source_node, nc_node * target_node, E edge_property);

	std::set<std::unique_ptr<nc_node>> nodes;
	std::set<std::unique_ptr<nc_edge>> edges;
};


template <typename N, typename E>
typename node_centric_graph<N, E>::nc_node * 
node_centric_graph<N, E>::create_node(N node_property)
{
	auto * node_ptr = new nc_node { {},{}, node_property };
	nodes.insert(std::unique_ptr<nc_node>(node_ptr));
	return node_ptr;
}

template <typename N, typename E>
typename node_centric_graph<N, E>::nc_edge * 
node_centric_graph<N, E>::create_edge(nc_node * source_node, nc_node * target_node, E edge_property)
{
	auto * edge_ptr = new nc_edge { source_node, target_node, edge_property };
	edges.insert(std::unique_ptr<nc_edge>(edge_ptr));
	source_node->outgoing.insert(edge_ptr);
	target_node->incoming.insert(edge_ptr);
	return edge_ptr;
}

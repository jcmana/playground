#pragma once

#include <memory>
#include <set>
#include <utility>

/// \brief		Graph specialization for edge traversing and adjacenct enumeration.
/// \desc		Graph specialization supporting:
///					- fast incoming/outgoing edges access
///					- directed edges
///					- custom node property
///					- custom edge property
template <typename NodeProperty, typename EdgeProperty>
struct node_centric_graph
{
	class node;
	class edge;

	struct node
	{
		template <typename ... Args>
		node(Args && ... node_property_args);

		std::set<edge *> outgoing;
		std::set<edge *> incoming;
		NodeProperty property;
	};

	struct edge
	{
		template <typename ... Args>
		edge(node * source_node, node * target_node, Args && ... edge_property_args);

		node * source;
		node * target;
		EdgeProperty property;
	};

	template <typename ... Args>
	node * create_node(Args && ... node_property_args);

	template <typename ... Args>
	edge * create_edge(node * source_node, node * target_node, Args && ... edge_property_args);

	std::set<std::unique_ptr<node>> nodes;
	std::set<std::unique_ptr<edge>> edges;
};

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric_graph<NodeProperty, EdgeProperty>::node::node(Args && ... node_property_args) :
	outgoing(),
	incoming(),
	property(std::forward<Args>(node_property_args)...)
{
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
node_centric_graph<NodeProperty, EdgeProperty>::edge::edge(node * source_node, node * target_node, Args && ... edge_property_args) :
	source(source_node),
	target(target_node),
	property(std::forward<Args>(edge_property_args)...)
{
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_graph<NodeProperty, EdgeProperty>::node *
node_centric_graph<NodeProperty, EdgeProperty>::create_node(Args && ... node_property_args)
{
	auto * node_ptr = new node(std::forward<Args>(node_property_args)...);
	nodes.insert(std::unique_ptr<node>(node_ptr));
	return node_ptr;
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_graph<NodeProperty, EdgeProperty>::edge *
node_centric_graph<NodeProperty, EdgeProperty>::create_edge(node * source_node, node * target_node, Args && ... edge_property_args)
{
	auto * edge_ptr = new edge(source_node, target_node, std::forward<Args>(edge_property_args)...);
	edges.insert(std::unique_ptr<edge>(edge_ptr));
	source_node->outgoing.insert(edge_ptr);
	target_node->incoming.insert(edge_ptr);
	return edge_ptr;
}

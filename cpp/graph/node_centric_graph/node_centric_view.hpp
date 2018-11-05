#pragma once

#include <vector>

#include "../../proxy_container/proxy_container/proxy_container.hpp"
#include "node_centric.hpp"

template <typename NodeProperty, typename EdgeProperty>
class node_centric_view
{
public:
	class node_iterator;
	class edge_iterator;

	// viewed graph types
	using graph = node_centric_graph<NodeProperty, EdgeProperty>;
	using graph_node_type = typename graph::node;
	using graph_edge_type = typename graph::edge;

	// proxy container types:
	using node_container_type = proxy_container<graph::container_type<graph_node_type *>, node_iterator>;
	using edge_container_type = proxy_container<graph::container_type<graph_edge_type *>, edge_iterator>;

public:
	class node_iterator
	{
	public:
		// constructor (by default initializes false state)
		node_iterator(graph_node_type * node_ptr = nullptr);

		node_iterator & follow(const EdgeProperty & edge_property);
		node_iterator & retreat(const EdgeProperty & edge_property);

		NodeProperty & operator *();
		NodeProperty * operator ->();

		// node edges proxy containers:
		edge_container_type outgoing_edges();
		//edge_container_type incoming_edges();

	private:
		/// \brief		Finds edge with appropriate property in given container.
		graph_node_type * find_edge_if(graph::container_type<graph_edge_type *> & edges, const EdgeProperty & edge_property);

	private:
		graph_node_type * m_node_ptr;

	private:
		friend node_centric_view;
		friend edge_iterator;
	};

	class edge_iterator
	{
	public:
		edge_iterator(graph_edge_type * edge_ptr);

		EdgeProperty & operator *();
		EdgeProperty * operator ->();

		node_iterator source();
		node_iterator target();

	private:
		graph_edge_type * m_edge_ptr;

	private:
		friend node_centric_view;
		friend node_iterator;
	};

	template <typename ... Args>
	node_iterator create_node(Args ... node_property_args);

	template <typename ... Args>
	edge_iterator create_edge(node_iterator & source, node_iterator & target, Args ... edge_property_args);

private:
	node_centric_graph<NodeProperty, EdgeProperty> m_data;
};


// === node_centric_view::node_iterator implementation:

template <typename NodeProperty, typename EdgeProperty>
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::node_iterator(graph_node_type * node_ptr = nullptr) :
	m_node_ptr(node_ptr)
{
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::node_iterator &
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::follow(const EdgeProperty & edge_property)
{
	m_node_ptr = find_edge_if(m_node_ptr->outgoing, edge_property);

	return (*this);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::node_iterator &
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::retreat(const EdgeProperty & edge_property)
{
	return (*this);
}

template <typename NodeProperty, typename EdgeProperty>
NodeProperty & 
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::operator *()
{
	return m_node_ptr->property;
}

template <typename NodeProperty, typename EdgeProperty>
NodeProperty * 
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::operator ->()
{
	return &(m_node_ptr->property);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::edge_container_type
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::outgoing_edges()
{
	return node_centric_view::edge_container_type(m_node_ptr->outgoing);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::graph_node_type *
node_centric_view<NodeProperty, EdgeProperty>::node_iterator::find_edge_if(graph::container_type<graph_edge_type *>  & edges, const EdgeProperty & edge_property)
{
	for (graph_edge_type * edge_ptr : edges)
	{
		if (edge_ptr->property == edge_property)
		{
			return edge_ptr->target;
		}
	}

	return nullptr;
}


// === node_centric_view::edge_iterator implementation:

template <typename NodeProperty, typename EdgeProperty>
node_centric_view<NodeProperty, EdgeProperty>::edge_iterator::edge_iterator(graph_edge_type * edge_ptr) :
	m_edge_ptr(edge_ptr)
{
}

template <typename NodeProperty, typename EdgeProperty>
EdgeProperty & 
node_centric_view<NodeProperty, EdgeProperty>::edge_iterator::operator *()
{
	return m_edge_ptr->property;
}

template <typename NodeProperty, typename EdgeProperty>
EdgeProperty * 
node_centric_view<NodeProperty, EdgeProperty>::edge_iterator::operator ->()
{
	return &(m_edge_ptr->property);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::node_iterator
node_centric_view<NodeProperty, EdgeProperty>::edge_iterator::source()
{
	return node_iterator(m_edge_ptr->source);
}

template <typename NodeProperty, typename EdgeProperty>
typename node_centric_view<NodeProperty, EdgeProperty>::node_iterator
node_centric_view<NodeProperty, EdgeProperty>::edge_iterator::target()
{
	return node_iterator(m_edge_ptr->target);
}


// === node_centric_view implementation:

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_view<NodeProperty, EdgeProperty>::node_iterator
node_centric_view<NodeProperty, EdgeProperty>::create_node(Args ... node_property_args)
{
	graph_node_type * node_ptr = m_data.create_node(std::forward<Args>(node_property_args) ...);
	return node_iterator(node_ptr);
}

template <typename NodeProperty, typename EdgeProperty>
template <typename ... Args>
typename node_centric_view<NodeProperty, EdgeProperty>::edge_iterator
node_centric_view<NodeProperty, EdgeProperty>::create_edge(node_iterator & source, node_iterator & target, Args ... edge_property_args)
{
	graph_edge_type * edge_ptr = m_data.create_edge(source.m_node_ptr, target.m_node_ptr, std::forward<Args>(edge_property_args) ...);
	return edge_iterator(edge_ptr);
}

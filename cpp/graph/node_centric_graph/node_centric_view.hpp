#pragma once

#include <vector>

#include "../../proxy_container/proxy_container/proxy_container.hpp"
#include "node_centric.hpp"

template <typename NodeProperty, typename EdgeProperty>
class node_centric_view
{
public:
	using graph = node_centric_graph<NodeProperty, EdgeProperty>;
	using graph_node_type = typename graph::node;
	using graph_edge_type = typename graph::edge;

public:
	class node_iterator;
	class edge_iterator;

	class node_iterator
	{
	public:
		using node_container_type = proxy_container<std::vector<graph_node_type *>, node_iterator>;
		using edge_container_type = proxy_container<std::vector<graph_edge_type *>, edge_iterator>;

	public:
		node_iterator(graph_node_type * node_ptr = nullptr) :
			m_node_ptr(node_ptr)
		{
		}

		node_iterator & follow(const EdgeProperty & edge_property)
		{
			return (*this);
		}

		node_iterator & retreat(const EdgeProperty & edge_property)
		{
			return (*this);
		}

		NodeProperty & operator *()
		{
			return m_node_ptr->property;
		}

		NodeProperty * operator ->()
		{
			return &(m_node_ptr->property);
		}

		edge_container_type outgoing_edges()
		{
			return edge_container_type(m_node_ptr->outgoing);
		}

	private:
		graph_node_type * find_edge_if(edge_container_type & edges)
		{
			for (auto it_edge : edges)
			{
				if ((*it_edge) == edge_property)
				{
					return it_edge.target().m_node_ptr;
				}
			}

			return nullptr;
		}

	private:
		graph_node_type * m_node_ptr;

	private:
		friend node_centric_view;
		friend edge_iterator;
	};

	class edge_iterator
	{
	public:
		edge_iterator(graph_edge_type * edge_ptr) :
			m_edge_ptr(edge_ptr)
		{
		}

		EdgeProperty & operator *()
		{
			return m_edge_ptr->property;
		}

		EdgeProperty * operator ->()
		{
			return &(m_edge_ptr->property);
		}

		node_iterator source()
		{
			return node_iterator(m_edge_ptr->source);
		}

		node_iterator target()
		{
			return node_iterator(m_edge_ptr->target);
		}

	private:
		graph_edge_type * m_edge_ptr;

	private:
		friend node_centric_view;
		friend node_iterator;
	};

	template <typename ... Args>
	node_iterator create_node(Args ... node_property_args)
	{
		graph_node_type * node_ptr = m_data.create_node(std::forward<Args>(node_property_args) ...);
		return node_iterator(node_ptr);
	}

	template <typename ... Args>
	edge_iterator create_edge(node_iterator & source, node_iterator & target, Args ... edge_property_args)
	{
		graph_edge_type * edge_ptr = m_data.create_edge(source.m_node_ptr, target.m_node_ptr, std::forward<Args>(edge_property_args) ...);
		return edge_iterator(edge_ptr);
	}

private:
	node_centric_graph<NodeProperty, EdgeProperty> m_data;
};


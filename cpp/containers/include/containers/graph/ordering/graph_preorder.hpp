#pragma once

#include "../iterators/graph_preorder_iterator.hpp"

template<typename Graph>
class graph_preorder
{
public:
	using graph = Graph;
	using graph_node = typename graph::node;
	using graph_edge = typename graph::edge;

	using iterator = typename graph_preorder_iterator<graph>;

public:
	graph_preorder(graph * graph_ptr, graph_node * node_ptr) :
		m_graph_ptr(graph_ptr),
		m_graph_node_ptr(node_ptr)
	{
	}

	iterator begin()
	{
		return iterator(m_graph_ptr, m_graph_node_ptr);
	}

	iterator end()
	{
		return iterator(m_graph_ptr);
	}

private:
	graph * m_graph_ptr;
	graph_node * m_graph_node_ptr = nullptr;
};

template<typename Graph>
typename graph_preorder<Graph> 
preorder(typename Graph * graph_ptr, typename graph_preorder<Graph>::graph_node * node_ptr)
{
	return graph_preorder<Graph>(graph_ptr, node_ptr);
}
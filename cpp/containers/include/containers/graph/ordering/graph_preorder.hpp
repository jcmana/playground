#pragma once

#include "../iterators/graph_preorder_iterator.hpp"

/// \brief		Defines preorder traversal on graph.
template<typename Graph>
class graph_preorder
{
public:
	using graph = Graph;
	using node = typename graph::node;
	using edge = typename graph::edge;

	using iterator = typename graph_preorder_iterator<graph>;

public:
	/// \brief		Defines preorder on `graph_ptr` starting at `node_ptr`
	graph_preorder(graph * graph_ptr, node * node_ptr);

	/// \brief		Iterator to the beggining of preodered `graph.
	iterator begin();
	
	/// \brief		Iterator to the end of preodered `graph.
	iterator end();

private:
	graph * m_graph_ptr = nullptr;
	node * m_node_ptr = nullptr;
};

/// \brief		Creates preorder traversal on `graph_ptr` starting at `node_ptr`.
template<typename Graph>
typename graph_preorder<Graph>
preorder(typename Graph * graph_ptr, typename graph_preorder<Graph>::node * node_ptr);

#pragma region graph_preorder implementation:

template<typename Graph>
graph_preorder<Graph>::graph_preorder(graph * graph_ptr, node * node_ptr) :
	m_graph_ptr(graph_ptr),
	m_node_ptr(node_ptr)
{
}

template<typename Graph>
typename graph_preorder<Graph>::iterator 
graph_preorder<Graph>::begin()
{
	return iterator(m_graph_ptr, m_node_ptr);
}

template<typename Graph>
typename graph_preorder<Graph>::iterator
graph_preorder<Graph>::end()
{
	return iterator(m_graph_ptr);
}

#pragma endregion

#pragma region preorder implementation:

template<typename Graph>
typename graph_preorder<Graph> 
preorder(typename Graph * graph_ptr, typename graph_preorder<Graph>::node * node_ptr)
{
	return graph_preorder<Graph>(graph_ptr, node_ptr);
}

#pragma endregion

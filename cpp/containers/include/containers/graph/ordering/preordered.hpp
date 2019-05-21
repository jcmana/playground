#pragma once

#include "../iterators/preorder_edge_iterator.hpp"

namespace containers {
namespace graph {

/// \brief		Defines preorder traversal on `Graph` over its `edge`s.
template<typename Graph>
class preordered
{
public:
	using iterator = typename preorder_edge_iterator<Graph>;

public:
	/// \brief		Defines preorder on `graph_ptr` starting at `node_ptr`
	preordered(typename Graph * graph_ptr, typename Graph::node * node_ptr);

	/// \brief		Iterator to the beggining of preodered `graph.
	iterator begin();
	
	/// \brief		Iterator to the end of preodered `graph.
	iterator end();

private:
	typename Graph * m_graph_ptr = nullptr;
	typename Graph::node * m_node_ptr = nullptr;
};

/// \brief		Creates preorder traversal on `graph_ptr` starting at `node_ptr`.
///
/// Iterator over all `edge`s with unique path accessible from initial `node` sorted using preorder. Unique
/// path is a path of `edge`s leading from initial `node` to the current `edge` under iterator. This could 
/// result in single `edge` being visited multiple times as it lies on multiple unique paths.
///
/// `preorder()` usage as standard container:
///	\code
/// graph g = ...;							// build the graph's nodes and edges
/// graph::node * initial_node_ptr = ...;	// select initial node
/// 
/// for (graph::edge * edge_ptr : preorder(&g, initial_node_ptr))
/// {
///		// handle current edge * edge_ptr
/// }
/// \endcode
template<typename Graph>
typename preordered<Graph>
preorder(typename Graph * graph_ptr, typename Graph::node * node_ptr);

#pragma region graph_preorder implementation:

template<typename Graph>
preordered<Graph>::preordered(typename Graph * graph_ptr, typename Graph::node * node_ptr) :
	m_graph_ptr(graph_ptr),
	m_node_ptr(node_ptr)
{
}

template<typename Graph>
typename preordered<Graph>::iterator
preordered<Graph>::begin()
{
	return iterator(m_graph_ptr, m_node_ptr);
}

template<typename Graph>
typename preordered<Graph>::iterator
preordered<Graph>::end()
{
	return iterator(m_graph_ptr);
}

#pragma endregion

#pragma region preorder implementation:

template<typename Graph>
typename preordered<Graph>
preorder(typename Graph * graph_ptr, typename Graph::node * node_ptr)
{
	return preordered<Graph>(graph_ptr, node_ptr);
}

#pragma endregion

} // namespace graph
} // namespace containers

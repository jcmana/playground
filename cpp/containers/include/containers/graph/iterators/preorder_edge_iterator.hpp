#pragma once

#include <vector>
#include <stack>

namespace containers {
namespace graph {

/// \brief		Preorder tree-like iterator over outgoing edges.
template<typename Graph>
class preorder_edge_iterator
{
public:
	/// \brief		Empty iterator.
	preorder_edge_iterator();

	/// \brief		Iterator at the end of `graph` `graph_ptr`.
	preorder_edge_iterator(typename Graph * graph_ptr);
	
	/// \brief		Iterator at `node` `node_ptr` of `graph` `graph_ptr`.
	preorder_edge_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr);

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_edge_iterator<Graph> & left, const preorder_edge_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_edge_iterator<Graph> & left, const preorder_edge_iterator<Graph> & right);

	/// \brief		Iterator pre-increment.
	preorder_edge_iterator & operator ++();
	
	/// \brief		Iterator post-increment.
	preorder_edge_iterator operator ++(int);

	/// \brief		Referenced `edge` member access.
	typename Graph::edge * operator ->();

	/// \brief		Referenced `edge`.
	typename Graph::edge * operator  *();

private:
	/// \brief		Increments this iterator.
	void increment();

	/// \brief		Expands outgoing `edge`s from `node_ptr` onto the stack.
	void expand(typename Graph::node * node_ptr);

private:
	/// \brief		`graph` container for iteration.
	Graph * m_graph_ptr;

	/// \brief		Preorder traversal implementation.
	///
	/// Contains `edge`s to iterate over, sorted from "visit last" to "visit first".
	std::stack<typename Graph::edge *, std::vector<typename Graph::edge *>> m_stack;
};

#pragma region graph_preorder_edge_iterator implementation:

template<typename Graph>
preorder_edge_iterator<Graph>::preorder_edge_iterator() :
	m_graph_ptr(nullptr)
{
}

template<typename Graph>
preorder_edge_iterator<Graph>::preorder_edge_iterator(typename Graph * graph_ptr) :
	m_graph_ptr(graph_ptr)
{
}

template<typename Graph>
preorder_edge_iterator<Graph>::preorder_edge_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr) :
	m_graph_ptr(graph_ptr)
{
	expand(node_ptr);
}

template<typename Graph>
typename preorder_edge_iterator<Graph> &
preorder_edge_iterator<Graph>::operator ++()
{
	increment();
	return (*this);
}

template<typename Graph>
typename preorder_edge_iterator<Graph>
preorder_edge_iterator<Graph>::operator ++(int)
{
	auto copy(*this);
	increment();
	return copy;
}

template<typename Graph>
typename Graph::edge *
preorder_edge_iterator<Graph>::operator ->()
{
	return m_stack.top();
}

template<typename Graph>
typename Graph::edge *
preorder_edge_iterator<Graph>::operator  *()
{
	return m_stack.top();
}

template<typename Graph>
void 
preorder_edge_iterator<Graph>::increment()
{
	typename Graph::edge * edge_ptr = m_stack.top();
	m_stack.pop();

	expand(edge_ptr->target);
}

template<typename Graph>
void 
preorder_edge_iterator<Graph>::expand(typename Graph::node * node_ptr)
{
	for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
	{
		m_stack.push(*edge_it);
	}
}

template<typename Graph>
bool 
operator ==(const preorder_edge_iterator<Graph> & left, const preorder_edge_iterator<Graph> & right)
{
	return (left.m_graph_ptr == right.m_graph_ptr && left.m_stack == right.m_stack);
}

template<typename Graph>
bool
operator !=(const preorder_edge_iterator<Graph> & left, const preorder_edge_iterator<Graph> & right)
{
	return !(left == right);
}

#pragma endregion

} // namespace graph
} // namespace containers

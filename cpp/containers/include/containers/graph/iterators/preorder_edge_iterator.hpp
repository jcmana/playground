#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief		Preorder iterator over outgoing edges.
template<typename Graph>
class preorder_edge_iterator
{
public:
	/// \brief		Initializes empty iterator.
	preorder_edge_iterator();
	
	/// \brief		Initializes iterator from `node_ptr`.
	preorder_edge_iterator(typename Graph::node * node_ptr);

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
	/// \brief		Increments the iterator.
	void increment();

	/// \brief		Expands the `node` onto the stack.
	void expand(typename Graph::node * node_ptr);

private:
	/// \brief		Preorder traversal implementation.
	///
	/// Contains `edge`s to iterate over, sorted from "visit last" to "visit first".
	std::vector<typename Graph::edge *> m_stack;
};

#pragma region preorder_edge_iterator implementation:

template<typename Graph>
preorder_edge_iterator<Graph>::preorder_edge_iterator()
{
}

template<typename Graph>
preorder_edge_iterator<Graph>::preorder_edge_iterator(typename Graph::node * node_ptr)
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
	return m_stack.back();
}

template<typename Graph>
typename Graph::edge *
preorder_edge_iterator<Graph>::operator  *()
{
	return m_stack.back();
}

template<typename Graph>
void 
preorder_edge_iterator<Graph>::increment()
{
	typename Graph::edge * edge_ptr = m_stack.back();
	m_stack.pop_back();

	expand(edge_ptr->target);
}

template<typename Graph>
void 
preorder_edge_iterator<Graph>::expand(typename Graph::node * node_ptr)
{
	for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
	{
		m_stack.push_back(*edge_it);
	}
}

template<typename Graph>
bool 
operator ==(const preorder_edge_iterator<Graph> & left, const preorder_edge_iterator<Graph> & right)
{
	return (left.m_stack == right.m_stack);
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

#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief		Preorder iterator over graph `node`s.
template<typename Graph>
class preorder_node_iterator
{
public:
	/// \brief		Iterator at the end of `graph` `graph_ptr`.
	preorder_node_iterator();

	/// \brief		Iterator at `node` `node_ptr` of `graph` `graph_ptr`.
	preorder_node_iterator(typename Graph::node * node_ptr);

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right);

	/// \brief		Iterator pre-increment.
	preorder_node_iterator & operator ++();

	/// \brief		Iterator post-increment.
	preorder_node_iterator operator ++(int);

	/// \brief		Referenced `edge` member access.
	typename Graph::node * operator ->();

	/// \brief		Referenced `edge`.
	typename Graph::node * operator  *();

private:
	/// \brief		Increments the iterator.
	void increment();

private:
	/// \brief		Preorder traversal implementation.
	std::vector<typename Graph::node *> m_stack;
};

#pragma region preorder_node_iterator implementation:

template<typename Graph>
preorder_node_iterator<Graph>::preorder_node_iterator()
{
}

template<typename Graph>
preorder_node_iterator<Graph>::preorder_node_iterator(typename Graph::node * node_ptr)
{
	m_stack.push_back(node_ptr);
}

template<typename Graph>
typename preorder_node_iterator<Graph> &
preorder_node_iterator<Graph>::operator ++()
{
	increment();
	return (*this);
}

template<typename Graph>
typename preorder_node_iterator<Graph>
preorder_node_iterator<Graph>::operator ++(int)
{
	auto copy(*this);
	increment();
	return copy;
}

template<typename Graph>
typename Graph::node *
preorder_node_iterator<Graph>::operator ->()
{
	return m_stack.back();
}

template<typename Graph>
typename Graph::node *
preorder_node_iterator<Graph>::operator  *()
{
	return m_stack.back();
}

template<typename Graph>
void
preorder_node_iterator<Graph>::increment()
{
	// Make current expansion node visited
	Graph::node * curr = m_stack.back();
	m_stack.pop_back();

	// Expand for each outgoing edge from current expansion node
	for (auto edge_it = (curr->outgoing).rbegin(); edge_it < (curr->outgoing).rend(); ++edge_it)
	{
		m_stack.push_back((*edge_it)->target);
	}
}

template<typename Graph>
bool
operator ==(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right)
{
	return left.m_stack == right.m_stack;
}

template<typename Graph>
bool
operator !=(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right)
{
	return !(left == right);
}

#pragma endregion

} // namespace graph
} // namespace containers

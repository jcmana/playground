#pragma once

#include <vector>
#include <stack>

/// \brief		Preorder tree-like iterator over outgoing edges.
template<typename Graph>
class graph_preorder_iterator
{
private:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	/// \brief		Empty iterator.
	graph_preorder_iterator();

	/// \brief		Iterator at the end of `graph` `graph_ptr`.
	graph_preorder_iterator(graph * graph_ptr);
	
	/// \brief		Iterator at `node` `node_ptr` of `graph` `graph_ptr`.
	graph_preorder_iterator(graph * graph_ptr, node * node_ptr);

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);

	/// \brief		Iterator pre-increment.
	graph_preorder_iterator & operator ++();
	
	/// \brief		Iterator post-increment.
	graph_preorder_iterator operator ++(int);

	/// \brief		Referenced `edge` member access.
	edge * operator ->();

	/// \brief		Referenced `edge`.
	edge * operator  *();

private:
	/// \brief		Increments this iterator.
	void increment();

	/// \brief		Expands outgoing `edge`s from `node_ptr` onto the stack.
	void expand(node * node_ptr);

private:
	/// \brief		`graph` container for iteration.
	graph * m_graph_ptr;

	/// \brief		Preorder traversal implementation.
	///
	/// Contains `edge`s to iterate over, sorted from "visit last" to "visit first".
	std::stack<edge *, std::vector<edge *>> m_stack;
};

#pragma region graph_preorder_iterator implementation:

template<typename Graph>
graph_preorder_iterator<Graph>::graph_preorder_iterator() :
	m_graph_ptr(nullptr)
{
}

template<typename Graph>
graph_preorder_iterator<Graph>::graph_preorder_iterator(graph * graph_ptr) :
	m_graph_ptr(graph_ptr)
{
}

template<typename Graph>
graph_preorder_iterator<Graph>::graph_preorder_iterator(graph * graph_ptr, node * node_ptr) :
	m_graph_ptr(graph_ptr)
{
	expand(node_ptr);
}

template<typename Graph>
typename graph_preorder_iterator<Graph> &
graph_preorder_iterator<Graph>::operator ++()
{
	increment();
	return (*this);
}

template<typename Graph>
typename graph_preorder_iterator<Graph>
graph_preorder_iterator<Graph>::operator ++(int)
{
	auto copy(*this);
	increment();
	return copy;
}

template<typename Graph>
typename graph_preorder_iterator<Graph>::edge *
graph_preorder_iterator<Graph>::operator ->()
{
	return m_stack.top();
}

template<typename Graph>
typename graph_preorder_iterator<Graph>::edge *
graph_preorder_iterator<Graph>::operator  *()
{
	return m_stack.top();
}

template<typename Graph>
void 
graph_preorder_iterator<Graph>::increment()
{
	edge * edge_ptr = m_stack.top();
	m_stack.pop();

	expand(edge_ptr->target);
}

template<typename Graph>
void 
graph_preorder_iterator<Graph>::expand(node * node_ptr)
{
	for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
	{
		m_stack.push(*edge_it);
	}
}

template<typename Graph>
bool 
operator ==(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right)
{
	return (left.m_graph_ptr == right.m_graph_ptr && left.m_stack == right.m_stack);
}

template<typename Graph>
bool
operator !=(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right)
{
	return !(left == right);
}

#pragma endregion

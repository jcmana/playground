#pragma once

#include <vector>

/// \brief		Preorder tree-like iterator over outgoing edges.
template<typename Graph>
class graph_preorder_iterator
{
public:
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

	/// \brief		Referenced `edge` edge.
	edge * operator  *();

private:
	void increment();
	void expand(node * node_ptr);

private:
	graph * m_graph_ptr;
	std::vector<edge *> m_stack;
};

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
	return m_stack.back();
}

template<typename Graph>
typename graph_preorder_iterator<Graph>::edge *
graph_preorder_iterator<Graph>::operator  *()
{
	return m_stack.back();
}

template<typename Graph>
void 
graph_preorder_iterator<Graph>::increment()
{
	edge * edge_ptr = m_stack.back();
	m_stack.pop_back();

	expand(edge_ptr->target);
}

template<typename Graph>
void 
graph_preorder_iterator<Graph>::expand(node * node_ptr)
{
	for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
	{
		m_stack.push_back(*edge_it);
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

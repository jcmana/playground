#pragma once

#include <vector>
#include <stack>

namespace containers {
namespace graph {

/// \brief		Preorder tree-like iterator over outgoing edges.
template<typename Graph>
class preorder_node_iterator
{
public:
	/// \brief		Empty iterator.
	preorder_node_iterator();

	/// \brief		Iterator at the end of `graph` `graph_ptr`.
	preorder_node_iterator(typename Graph * graph_ptr);

	/// \brief		Iterator at `node` `node_ptr` of `graph` `graph_ptr`.
	preorder_node_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr);

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
	struct expansion
	{
		/// \brief		`edge` expansion leading to the `node`.
		typename Graph::edge * edge_ptr;

		/// \brief		`node` to iterate over.
		typename Graph::node * node_ptr;

		/// \brief		depth (or distance) of the `node_ptr` below initial node.
		unsigned int depth;
	};

private:
	/// \brief		Increments this iterator.
	void increment();

	/// \brief		Expands outgoing `edge`s from `node_ptr` onto the stack.
	void expand(const typename expansion & e);

private:
	/// \brief		`graph` container for iteration.
	Graph * m_graph_ptr;

	/// \brief		Preorder traversal implementation.
	std::vector<typename expansion> m_stack;

	/// \brief		Graph path leading from initial `node` to the current `node`.
	std::vector<typename Graph::edge *> m_path;
};

#pragma region graph_preorder_node_iterator implementation:

template<typename Graph>
preorder_node_iterator<Graph>::preorder_node_iterator() :
	m_graph_ptr(nullptr)
{
}

template<typename Graph>
preorder_node_iterator<Graph>::preorder_node_iterator(typename Graph * graph_ptr) :
	m_graph_ptr(graph_ptr)
{
}

template<typename Graph>
preorder_node_iterator<Graph>::preorder_node_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr) :
	m_graph_ptr(graph_ptr)
{
	expansion e;
	e.edge_ptr = nullptr;
	e.node_ptr = node_ptr;
	e.depth = 0;

	m_stack.push_back(e);
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
	return m_stack.back().node_ptr;
}

template<typename Graph>
typename Graph::node *
preorder_node_iterator<Graph>::operator  *()
{
	return m_stack.back().node_ptr;
}

template<typename Graph>
void
preorder_node_iterator<Graph>::increment()
{
	expansion curr = m_stack.back();
	m_stack.pop_back();

	expand(curr);
}

template<typename Graph>
void
preorder_node_iterator<Graph>::expand(const typename expansion & curr)
{
	typename Graph::node * node_ptr = curr.node_ptr;

	// Expand the node:
	for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
	{
		expansion next;
		next.edge_ptr = (*edge_it);
		next.node_ptr = (*edge_it)->target;
		next.depth = (curr.depth + 1);

		m_stack.push_back(next);
	}

	// Update the path:
	{
		expansion curr = m_stack.back();

		m_path.resize(curr.depth);
		m_path[curr.depth - 1] = curr.edge_ptr;
	}
}

template<typename Graph>
bool
operator ==(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right)
{
	//return (left.m_graph_ptr == right.m_graph_ptr && left.m_stack == right.m_stack);
	return false;
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

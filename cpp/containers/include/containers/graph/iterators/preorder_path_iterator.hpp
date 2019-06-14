#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief		Preorder tree-like iterator over outgoing edges.
template<typename Graph>
class preorder_path_iterator
{
public:
	using path = std::vector<typename Graph::edge *>;

public:
	preorder_path_iterator();

	preorder_path_iterator(typename Graph::edge * edge_ptr);

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_path_iterator<Graph> & left, const preorder_path_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_path_iterator<Graph> & left, const preorder_path_iterator<Graph> & right);

	/// \brief		Iterator pre-increment.
	preorder_path_iterator & operator ++();

	/// \brief		Iterator post-increment.
	preorder_path_iterator operator ++(int);

	/// \brief		Unique path in `graph`.
	path operator  *();

private:
	struct stack_node
	{
		/// \brief		`edge` expansion leading to the `node`.
		typename Graph::edge * edge_ptr;

		/// \brief		`node` to iterate over.
		typename Graph::node * node_ptr;

		/// \brief		Depth (or distance) of the `node_ptr` below initial node.
		unsigned int depth;
	};

private:
	/// \brief		Increments this iterator.
	void increment();

private:
	/// \brief		Preorder traversal implementation.
	std::vector<stack_node> m_stack;

	/// \brief		Graph path leading from initial `node` to the current `node`.
	path m_path;
};

#pragma region preorder_path_iterator implementation:

template<typename Graph>
preorder_path_iterator<Graph>::preorder_path_iterator()
{
	// Prepare terminating node on the stack
	{
		stack_node exit;
		exit.node_ptr = nullptr;
		exit.edge_ptr = nullptr;
		exit.depth = 0;

		m_stack.push_back(exit);
	}
}

template<typename Graph>
preorder_path_iterator<Graph>::preorder_path_iterator(typename Graph::edge * edge_ptr)
{
	// Prepare terminating node on the stack:
	stack_node exit;
	exit.node_ptr = nullptr;
	exit.edge_ptr = nullptr;
	exit.depth = 0;

	m_stack.push_back(exit);

	// Prepare initial node on the stack:
	stack_node init;
	init.node_ptr = edge_ptr->target;
	init.edge_ptr = edge_ptr;
	init.depth = 1;

	m_stack.push_back(init);
}

template<typename Graph>
typename preorder_path_iterator<Graph> &
preorder_path_iterator<Graph>::operator ++()
{
	increment();
	return (*this);
}

template<typename Graph>
typename preorder_path_iterator<Graph>
preorder_path_iterator<Graph>::operator ++(int)
{
	auto copy(*this);
	increment();
	return copy;
}

template<typename Graph>
typename preorder_path_iterator<Graph>::path
preorder_path_iterator<Graph>::operator  *()
{
	stack_node curr = m_stack.back();

	path complete_path = m_path;
	complete_path.resize(curr.depth);
	complete_path[curr.depth - 1] = curr.edge_ptr;

	return complete_path;
}

template<typename Graph>
void
preorder_path_iterator<Graph>::increment()
{
	// Pickup the last visited node
	stack_node curr = m_stack.back();
	m_stack.pop_back();

	// Node expansion template
	stack_node next;
	next.edge_ptr = nullptr;
	next.node_ptr = nullptr;
	next.depth = (curr.depth + 1);

	// Update path
	m_path.resize(curr.depth);
	m_path[curr.depth - 1] = curr.edge_ptr;

	// Expand for each outgoing edge from current expansion node
	for (auto edge_it = (curr.node_ptr)->outgoing.rbegin(); edge_it < (curr.node_ptr)->outgoing.rend(); ++edge_it)
	{
		next.edge_ptr = (*edge_it);
		next.node_ptr = (*edge_it)->target;

		// Push expanded node onto the stack
		m_stack.push_back(next);
	}
}

template<typename Graph>
bool
operator ==(const preorder_path_iterator<Graph> & left, const preorder_path_iterator<Graph> & right)
{
	auto stack_node_eq_predicate = [](typename const preorder_path_iterator<Graph>::stack_node & left, typename const preorder_path_iterator<Graph>::stack_node & right)
	{
		bool node_eq = (left.node_ptr == right.node_ptr);
		bool edge_eq = (left.edge_ptr == right.edge_ptr);
		bool depth_eq = (left.depth == right.depth);
		return (node_eq && edge_eq && depth_eq);
	};

	return std::equal(left.m_stack.begin(), left.m_stack.end(), right.m_stack.begin(), right.m_stack.end(), stack_node_eq_predicate);
}

template<typename Graph>
bool
operator !=(const preorder_path_iterator<Graph> & left, const preorder_path_iterator<Graph> & right)
{
	return !(left == right);
}

#pragma endregion

} // namespace graph
} // namespace containers

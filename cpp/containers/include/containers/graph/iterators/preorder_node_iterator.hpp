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

	const auto & path() const;

private:
	struct stack_node
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

private:
	/// \brief		`graph` container for iteration.
	Graph * m_graph_ptr;

	/// \brief		Preorder traversal implementation.
	std::vector<stack_node> m_stack;

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
	stack_node init;
	init.node_ptr = node_ptr;
	init.edge_ptr = nullptr;
	init.depth = 0;

	m_stack.push_back(init);
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
const auto & 
preorder_node_iterator<Graph>::path() const
{
	return m_path;
}

template<typename Graph>
void
preorder_node_iterator<Graph>::increment()
{
	stack_node curr = m_stack.back();
	
	// Make current expansion node visited
	m_stack.pop_back();

	// Expand for each outgoing edge from current expansion node
	for (auto edge_it = (curr.node_ptr)->outgoing.rbegin(); edge_it < (curr.node_ptr)->outgoing.rend(); ++edge_it)
	{
		stack_node next;
		next.edge_ptr = (*edge_it);
		next.node_ptr = (*edge_it)->target;
		next.depth = (curr.depth + 1);

		m_stack.push_back(next);
	}

	// FOLLOWING CODE IS WHOLE LOT INCORRECT:
	// - increment() differs for initial node, in-between nodes and last node
	// - increment() preciselly knows when it is on last node from its implementations
	// - m_stack and m_path in initial state and finished state are inconsistent
	//		-- initial: m_stack has 1 element, m_path is empty
	//		-- finished: m_stack is empty, m_path is empty

	// Check stack underflow 
	if (m_stack.size() == 0)
	{
		m_path.clear();
		return;
	}

	// Update path for current expansion node
	curr = m_stack.back();
	m_path.resize(curr.depth);
	m_path[curr.depth - 1] = curr.edge_ptr;

}

template<typename Graph>
bool
operator ==(const preorder_node_iterator<Graph> & left, const preorder_node_iterator<Graph> & right)
{
	auto stack_node_eq_predicate = [](typename const preorder_node_iterator<Graph>::stack_node & left, typename const preorder_node_iterator<Graph>::stack_node & right)
	{
		bool node_eq = (left.node_ptr == right.node_ptr);
		bool edge_eq = (left.edge_ptr == right.edge_ptr);
		bool depth_eq = (left.depth == right.depth);
		return (node_eq && edge_eq && depth_eq);
	};

	bool graph_eq = (left.m_graph_ptr == right.m_graph_ptr);
	bool stack_eq = std::equal(left.m_stack.begin(), left.m_stack.end(), right.m_stack.begin(), right.m_stack.end(), stack_node_eq_predicate);
	bool path_eq = (left.m_path == right.m_path);

	return (graph_eq && stack_eq && path_eq);
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

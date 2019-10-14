#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief		Preorder iterator over unique paths in graph.
template<typename Graph>
class preorder_path_iterator
{
public:
	/// \brief		Unique ordered sequence of `edge`s through the graph.
	///
	/// First `edge`  of the path will always be the initial `edge`.
	using path = std::vector<typename Graph::edge *>;

public:
	/// \brief		Initializes empty iterator.
	preorder_path_iterator();

	/// \brief		Initializes iterator with starting `edge`.
	preorder_path_iterator(typename Graph::edge * edge_ptr);

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_path_iterator<Graph> & lhs, const preorder_path_iterator<Graph> & rhs);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_path_iterator<Graph> & lhs, const preorder_path_iterator<Graph> & rhs);

	/// \brief		Iterator pre-increment.
	preorder_path_iterator & operator ++();

	/// \brief		Iterator post-increment.
	preorder_path_iterator operator ++(int);

	/// \brief		Unique `path` through `graph`.
	///
	/// Dereference operator is lazy evaluated and threfore returns copy of `path` instead of 
	/// reference to a member. This means that repeated calls to the dereference will evaluate
	/// `path` every time.
	path operator  *();

private:
	/// \brief		Stack entry for algorithm using stack expansion implementing graph preorder.
	struct stack_node
	{
		/// \brief		`edge` expansion leading to the `node`.
		typename Graph::edge * edge_ptr;

		/// \brief		`node` to iterate over.
		typename Graph::node * node_ptr;

		/// \brief		Depth (or distance) of the `node_ptr` below initial node.
		unsigned int depth;

		/// \brief		Compares two stack nodes for equality.
		static bool compare(const typename preorder_path_iterator<Graph>::stack_node & lhs, const typename preorder_path_iterator<Graph>::stack_node & rhs);
	};

private:
	/// \brief		Pushes node on top of the stack.
	void push(const stack_node & node);

	/// \brief		Removes node from top of the stack and returns it.
	stack_node pop();

	/// \brief		Node on the of the stack.
	stack_node & top();

	/// \brief		Increments the iterator.
	void increment();

private:
	/// \brief		Preorder traversal implementation stack.
	std::vector<stack_node> m_stack;

	/// \brief		Graph path leading from initial `node` to the last expanded `node`.
	path m_path;
};

#pragma region preorder_path_iterator implementation:

template<typename Graph>
preorder_path_iterator<Graph>::preorder_path_iterator()
{
}

template<typename Graph>
preorder_path_iterator<Graph>::preorder_path_iterator(typename Graph::edge * edge_ptr)
{
	// Prepare initial node on the stack:
	stack_node init;
	init.node_ptr = edge_ptr->target;
	init.edge_ptr = edge_ptr;
	init.depth = 1;

	push(init);
}

template<typename Graph>
bool
operator ==(const preorder_path_iterator<Graph> & lhs, const preorder_path_iterator<Graph> & rhs)
{
	return std::equal(lhs.m_stack.begin(), lhs.m_stack.end(), rhs.m_stack.begin(), rhs.m_stack.end(), preorder_path_iterator<Graph>::stack_node::compare);
}

template<typename Graph>
bool
operator !=(const preorder_path_iterator<Graph> & lhs, const preorder_path_iterator<Graph> & rhs)
{
	return !(lhs == rhs);
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
	stack_node curr = top();

	// Ater each increment, path is missing last edge to the current node,
	// here we take that incomplete path, add one more edge and return complete copy
	path complete_path = m_path;
	complete_path.resize(curr.depth);
	complete_path[curr.depth - 1] = curr.edge_ptr;

	return complete_path;
}

template<typename Graph>
void 
preorder_path_iterator<Graph>::push(const stack_node & node)
{
	m_stack.push_back(node);
}

template<typename Graph>
typename preorder_path_iterator<Graph>::stack_node
preorder_path_iterator<Graph>::pop()
{
	stack_node curr = top();
	m_stack.pop_back();
	return curr;
}

template<typename Graph>
typename preorder_path_iterator<Graph>::stack_node &
preorder_path_iterator<Graph>::top()
{
	return m_stack.back();
}

template<typename Graph>
void
preorder_path_iterator<Graph>::increment()
{
	// Pop the last visited node
	stack_node curr = pop();

	// Node expansion template
	stack_node next;
	next.edge_ptr = nullptr;
	next.node_ptr = nullptr;
	next.depth = (curr.depth + 1);

	// Update path
	m_path.resize(curr.depth);
	m_path[curr.depth - 1] = curr.edge_ptr;

	// Expand each outgoing edge from current stack node (in reversed order)
	for (auto edge_it = (curr.node_ptr)->outgoing.rbegin(); edge_it < (curr.node_ptr)->outgoing.rend(); ++edge_it)
	{
		next.edge_ptr = (*edge_it);
		next.node_ptr = (*edge_it)->target;

		// Push expanded node onto the stack
		push(next);

#ifndef NDEBUG
		// Verify node won't create graph cycle:

		/* doesn't work
		for (auto it = m_stack.crbegin(); it != m_stack.crend(); ++it)
		{
			//if (stack_node::operator ==((*it), node))
			if (stack_node::compare((*it), node))
			{
				assert(true && "This node creates cyclic path");
			}
		}
		*/
#endif
	}
}

template<typename Graph>
bool 
preorder_path_iterator<Graph>::stack_node::compare(
	const typename preorder_path_iterator<Graph>::stack_node & lhs, 
	const typename preorder_path_iterator<Graph>::stack_node & rhs)
{
	bool node_eq = (lhs.node_ptr == rhs.node_ptr);
	bool edge_eq = (lhs.edge_ptr == rhs.edge_ptr);
	bool depth_eq = (lhs.depth == rhs.depth);
	
	return (node_eq && edge_eq && depth_eq);
}

#pragma endregion

} // namespace graph
} // namespace containers

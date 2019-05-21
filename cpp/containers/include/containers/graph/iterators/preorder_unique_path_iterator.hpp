#pragma once

#include <iostream>

#include <vector>
#include <stack>

#include "preorder_edge_iterator.hpp"

namespace containers {
namespace graph {

template<typename Graph>
class preorder_unique_path_iterator
{
public:
	preorder_unique_path_iterator() :
		m_graph_ptr(nullptr)
	{
	}

	preorder_unique_path_iterator(typename Graph * graph_ptr) :
		m_graph_ptr(graph_ptr)
	{
	}

	preorder_unique_path_iterator(typename Graph * graph_ptr, typename Graph::node * node_ptr) :
		m_graph_ptr(graph_ptr)
	{
		int path_number = 0;
		path_number += expand(node_ptr);
		m_path.push_back(m_stack.top());
		fork(path_number);
	}

	/// \brief		Compares iterators for equality.
	template<typename Graph>
	friend bool operator ==(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right);

	/// \brief		Compares iterators for in-equality.
	template<typename Graph>
	friend bool operator !=(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right);

	preorder_unique_path_iterator & operator ++()
	{
		increment();
		return (*this);
	}

	typename Graph::edge * operator ->()
	{
		return m_stack.top();
	}

	typename Graph::edge * operator  *()
	{
		return m_stack.top();
	}

	const std::vector<typename Graph::edge *> & path() const
	{
		return m_path;
	}

private:
	void increment()
	{
		int path_number = 0;

		typename Graph::edge * edge_ptr = m_stack.top();

		// Remove one edge and decrease path number
		m_stack.pop();
		path_number -= 1;

		// Expand the edge and increase the path number
		path_number += expand(edge_ptr->target);

		if (path_number < 0)
		{
			join();
			return;
		}

		if (path_number > 0)
		{
			fork(path_number);
			m_path.push_back(m_stack.top());
			return;
		}

		if (path_number == 0)
		{
			m_path.push_back(m_stack.top());
			return;
		}
	}

	int expand(typename Graph::node * node_ptr)
	{
		for (auto edge_it = (node_ptr->outgoing).rbegin(); edge_it < (node_ptr->outgoing).rend(); ++edge_it)
		{
			m_stack.push(*edge_it);
		}

		return node_ptr->outgoing.size();
	}

	void fork(int n_times)
	{
		for (int n = 0; n < n_times; ++n)
		{
			m_path_branch.push_back(std::distance(m_path.begin(), std::prev(m_path.end())));
		}
	}

	void join()
	{
		auto it = std::next(m_path.begin() + m_path_branch.back());
		m_path.erase(it, m_path.end());
		m_path_branch.pop_back();
	}

private:
	Graph * m_graph_ptr;
	std::stack<typename Graph::edge *, std::vector<typename Graph::edge *>> m_stack;
	std::vector<typename Graph::edge *> m_path;
	std::vector<typename std::vector<typename Graph::edge *>::iterator::difference_type> m_path_branch;
};

template<typename Graph>
bool operator ==(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right)
{
	return (left.m_stack == right.m_stack);
}

/// \brief		Compares iterators for in-equality.
template<typename Graph>
bool operator !=(const preorder_unique_path_iterator<Graph> & left, const preorder_unique_path_iterator<Graph> & right)
{
	return !(left == right);
}

} // namespace graph
} // namespace containers

#pragma once

#include <vector>

template<typename Graph>
class graph_preorder_iterator_stacked
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	graph_preorder_iterator_stacked()
	{
	}

	graph_preorder_iterator_stacked(node * node_ptr)
	{
		expand(node_ptr);
	}

	operator bool()
	{
		return (m_stack.size() > 0);
	}

	graph_preorder_iterator_stacked & operator ++()
	{
		increment();
		return (*this);
	}

	edge * operator ->()
	{
		return m_stack.back();
	}

	edge * operator  *()
	{
		return m_stack.back();
	}

private:
	void increment()
	{
		edge * edge_ptr = m_stack.back();
		m_stack.pop_back();

		expand(edge_ptr->target);
	}

	void expand(node * node_ptr)
	{
		for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
		{
			m_stack.push_back(*edge_it);
		}
	}

private:
	std::vector<edge *> m_stack;

template<typename Graph>
friend bool operator ==(const graph_preorder_iterator_stacked<Graph> & left, const graph_preorder_iterator_stacked<Graph> & right);

template<typename Graph>
friend bool operator !=(const graph_preorder_iterator_stacked<Graph> & left, const graph_preorder_iterator_stacked<Graph> & right);
};

template<typename Graph>
bool 
operator ==(const graph_preorder_iterator_stacked<Graph> & left, const graph_preorder_iterator_stacked<Graph> & right)
{
	return (left.m_stack == right.m_stack);
}

template<typename Graph>
bool
operator !=(const graph_preorder_iterator_stacked<Graph> & left, const graph_preorder_iterator_stacked<Graph> & right)
{
	return !(left == right);
}
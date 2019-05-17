#pragma once

#include <vector>

template<typename Graph>
class graph_preorder_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	graph_preorder_iterator(graph * graph_ptr) :
		m_graph_ptr(graph_ptr)
	{
	}

	graph_preorder_iterator(graph * graph_ptr, node * node_ptr) :
		m_graph_ptr(graph_ptr)
	{
		expand(node_ptr);
	}

	graph_preorder_iterator & operator ++()
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
	graph * m_graph_ptr = nullptr;
	std::vector<edge *> m_stack;

template<typename Graph>
friend bool operator ==(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);

template<typename Graph>
friend bool operator !=(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);
};

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

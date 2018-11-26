#pragma once

template <typename Graph>
class sibling_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

	using iterator = typename graph::edge_container::iterator;

public:
	sibling_iterator(node * parent_node_ptr, iterator current) :
		m_parent_node_ptr(parent_node_ptr),
		m_current(current)
	{
	}

	bool operator ==(sibling_iterator & other)
	{
		return (m_parent_node_ptr == other.m_parent_node_ptr && m_current == other.m_current);
	}

	bool operator !=(sibling_iterator & other)
	{
		return !(operator ==(other));
	}

	sibling_iterator & operator ++()
	{
		++m_current;
		return (*this);
	}

	node * operator ->()
	{
		return (*m_current)->target;
	}

	node * operator *()
	{
		return (*m_current)->target;
	}

private:


private:
	node * m_parent_node_ptr;
	iterator m_current;
};
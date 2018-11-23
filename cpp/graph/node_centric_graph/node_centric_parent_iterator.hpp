#pragma once

template <typename Graph>
class parent_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	parent_iterator()
	{
	}

	parent_iterator(const preorder_iterator & it) :
		m_it(it)
	{
	}

	bool operator ==(preorder_iterator & other)
	{
		return (m_node_ptr == other.m_node_ptr);
	}

	bool operator !=(preorder_iterator & other)
	{
		return !(operator ==(other));
	}

	preorder_iterator & operator ++()
	{
		return (*this);
	}

	node * operator ->()
	{

	}

	node * operator *()
	{

	}

private:
	const preorder_iterator & m_it;
};
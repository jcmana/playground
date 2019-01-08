#pragma once

#include <cassert>
#include <tuple>
#include <list>
#include <vector>
#include <algorithm>

namespace containers {
namespace graph {


template <typename Graph>
class preorder_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	preorder_iterator() :
		m_node_ptr(nullptr)
	{
	}

	preorder_iterator(node * node_ptr) :
		m_node_ptr(node_ptr)
	{
		m_trail.push_back(std::make_tuple(m_node_ptr, node_ptr->outgoing.begin(), node_ptr->outgoing.end()));
	}

/*
	bool operator ==(preorder_iterator & other)
	{
		return (m_node_ptr == other.m_node_ptr && m_trail == other.m_trail);
	}

	bool operator !=(preorder_iterator & other)
	{
		return !(operator ==(other));
	}
*/
	operator bool()
	{
		return (m_node_ptr != nullptr && m_trail.size() > 0);
	}

	preorder_iterator & operator ++()
	{
		assert(m_node_ptr != nullptr);
		assert(m_trail.size() > 0 && "Iterator is not incrementable");

		// emerge
		while (std::get<1>(m_trail.back()) == std::get<2>(m_trail.back()))
		{
			m_trail.pop_back();

			if (m_trail.size() == 0)
			{
				m_node_ptr = nullptr;
				return (*this);
			}
		}

		// current edge and node
		auto & it_current = std::get<1>(m_trail.back());

		assert(m_trail.size() != 0);
		assert(std::get<1>(m_trail.back()) != std::get<2>(m_trail.back()));

		auto * edge_ptr = (*it_current);
		auto * node_ptr = edge_ptr->target;
		m_node_ptr = node_ptr;

		// increment
		++it_current;

		// TODO: debug check: graph cycles (for each visited node check if it is already on stack)

		// submerge (if not leaf)
		if (node_ptr->outgoing.size() > 0)
		{
			m_trail.push_back(std::make_tuple(m_node_ptr, node_ptr->outgoing.begin(), node_ptr->outgoing.end()));
		}

		return (*this);
	}

	node * operator ->()
	{
		return m_node_ptr;
	}

	node * operator *()
	{
		return m_node_ptr;
	}

	std::vector<node *> trail() const
	{
		std::vector<node *> iterator_trail(m_trail.size());
		std::transform(m_trail.rbegin(), m_trail.rend(), iterator_trail.begin(), [](const breadcrumb & i)
		{
			return std::get<0>(i);
		});
		return iterator_trail;
	}

private:
	using node_iterator = typename graph::node_container::iterator;
	using edge_iterator = typename graph::edge_container::iterator;
	using breadcrumb = std::tuple<node *, edge_iterator, edge_iterator>;

private:
	node * m_node_ptr;
	std::vector<breadcrumb> m_trail;
};


} // namespace graph
} // namespace containers

#pragma once

#include <string>
#include <set>
#include <map>

#include "adjacency_graph_types.h"

class adjacency_graph;

using NodeProperty = std::string;
using EdgeProperty = int;
//template<typename NodeProperty, typename EdgeProperty>
class adjacency_graph_enumerator_iterator
{
private:
	using types = adjacency_graph_types<NodeProperty, EdgeProperty>;

public:
	using graph_type = adjacency_graph;
	using node_owner_type = types::node_owner_type;
	using edge_type = types::edge_type;
	using container_type = std::set<edge_type>;

	struct value_type
	{
		node_owner_type * source;
		node_owner_type * target;
		EdgeProperty property;
	};

protected:
	explicit adjacency_graph_enumerator_iterator(
		const container_type::iterator & iter,
		const EdgeProperty & property,
		bool end);

public:
	adjacency_graph_enumerator_iterator & next();
	bool equals(const adjacency_graph_enumerator_iterator & other);
	node_owner_type * deref();

private:
	container_type::iterator m_iterator;
	EdgeProperty m_property;
	bool m_end;

private:
	friend class adjacency_graph_enumerator;
};


adjacency_graph_enumerator_iterator::adjacency_graph_enumerator_iterator(
	const container_type::iterator & iter,
	const EdgeProperty & property,
	bool end
) :
	m_iterator(iter),
	m_property(property),
	m_end(end)
{
}

adjacency_graph_enumerator_iterator & adjacency_graph_enumerator_iterator::next()
{
	m_iterator++;

	while (m_iterator->second->property() != m_property)
	{
		m_iterator++;
	};

	return *this;
}

bool adjacency_graph_enumerator_iterator::equals(const adjacency_graph_enumerator_iterator & other)
{
	bool equal_edge_iterator = (m_iterator == other.m_iterator);
	bool equal_property = (m_property == other.m_property);
	bool equal_end = (m_end == other.m_end);

	return true;
}

adjacency_graph_enumerator_iterator::node_owner_type * adjacency_graph_enumerator_iterator::deref()
{
	return m_iterator->first;
}

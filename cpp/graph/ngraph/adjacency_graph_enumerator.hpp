#pragma once

#include <string>

#include "adjacency_graph_types.h"

class adjacency_graph_enumerator_iterator;

using NodeProperty = std::string;
using EdgeProperty = int;
//template<typename NodeProperty, typename EdgeProperty>
class adjacency_graph_enumerator
{
private:
	using types = adjacency_graph_types<NodeProperty, EdgeProperty>;

public:
	using node_owner_type = types::node_owner_type;
	using edge_owner_type = types::edge_owner_type;
	using edge_type = types::edge_type;
	using container_type = std::map<node_owner_type *, std::set<edge_type>>;
	using edge_container_type = std::set<edge_type>;

	using iterator = adjacency_graph_enumerator_iterator;

public:
	iterator begin();
	void end();

protected:
	explicit adjacency_graph_enumerator(container_type * edges, node_owner_type * source, const EdgeProperty & property);

private:
	container_type * m_edges;
	node_owner_type * m_source;
	EdgeProperty m_property;

private:
	friend class adjacency_graph;
};


#include "adjacency_graph_enumerator_iterator.hpp"

adjacency_graph_enumerator::iterator adjacency_graph_enumerator::begin()
{
	return iterator(m_edges->at(m_source).begin(), m_property, false);
}

void adjacency_graph_enumerator::end()
{

}

adjacency_graph_enumerator::adjacency_graph_enumerator(container_type * edges, node_owner_type * source, const EdgeProperty & property) :
	m_edges(edges),
	m_source(source),
	m_property(property)
{
}

#pragma once

#include <utility>

#include "adjacency_graph_node.hpp"
#include "adjacency_graph_edge.hpp"

template<typename NodeProperty, typename EdgeProperty>
class adjacency_graph_types
{
public:
	/// \brief		Node property owning type.
	using node_owner_type = adjacency_graph_node<NodeProperty>;
	/// \brief		Edge property owning type.
	using edge_owner_type = adjacency_graph_edge<EdgeProperty>;
	/// \brief		Edge owning type.
	using edge_type = std::pair<node_owner_type *, edge_owner_type *>;
};

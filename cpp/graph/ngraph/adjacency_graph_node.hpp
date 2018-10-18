#pragma once

#include <memory>

template<typename EdgeProperty>
class adjacency_graph_node
{
public:
	adjacency_graph_node(const EdgeProperty & property) :
		m_property(property)
	{
	}

	EdgeProperty & property()
	{
		return m_property;
	}

private:
	EdgeProperty m_property;
};

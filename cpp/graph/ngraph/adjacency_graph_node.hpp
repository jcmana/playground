#pragma once

#include <memory>

template<typename Property>
class adjacency_graph_node
{
public:
	adjacency_graph_node(const Property & property) :
		m_property(property)
	{
	}

	Property & property()
	{
		return m_property;
	}

private:
	Property m_property;
};

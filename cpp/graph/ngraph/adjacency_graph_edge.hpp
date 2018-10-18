#pragma once

template<typename Property>
class adjacency_graph_edge
{
public:
	adjacency_graph_edge(const Property & property) :
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

#pragma once

template<typename EdgeProperty>
class adjacency_graph_edge
{
public:
	adjacency_graph_edge(const EdgeProperty & property) :
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

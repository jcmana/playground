#pragma once

#include <vector>

template<typename Graph>
class preorder_iterator_stacked
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

public:
	preorder_iterator_stacked(const std::vector<edge *> & root)
	{
		m_path.push_back(root);
	}

	preorder_iterator_stacked & operator ++()
	{
		auto & current = m_path.back();

	}

private:
	std::vector<std::vector<edge *>> m_path;
};
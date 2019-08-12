#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <limits>
#include <utility>

namespace containers {
namespace graph {

template<typename Graph>
void dijkstra(const Graph & g, const typename Graph::node * source_node_ptr, const typename Graph::node * target_node_ptr)
{
	using node = typename Graph::node;
	using edge = typename Graph::edge;

	std::vector<const node *> Q;
	std::map<const node *, const node *> prev;
	std::map<const node *, int> dist;

	for (const node * node_ptr : g.nodes)
	{
		dist[node_ptr] = std::numeric_limits<int>::max();
		prev[node_ptr] = nullptr;
		Q.push_back(node_ptr);
	}

	dist[source_node_ptr] = 0;

	while (Q.empty() == false)
	{
		// Find element from Q with minimal entry in dist
		auto predicate = [&dist](const node * a, const node * b) -> bool
		{
			return dist[a] < dist[b];
		};
		auto it_min = std::min_element(Q.begin(), Q.end(), predicate);

		// Remember min node
		const node * min_node_ptr = *it_min;

		// Remove min element from the queue
		Q.erase(it_min);

		// Break if we found the target node
		if (min_node_ptr == target_node_ptr)
		{
			break;
		}
		
		// Update dist and prev maps with current node neigbors
		for (const edge * edge_ptr : min_node_ptr->outgoing)
		{
			int alt = dist[min_node_ptr] + edge_ptr->property;

			if (alt < dist[edge_ptr->target])
			{
				dist[edge_ptr->target] = alt;
				prev[edge_ptr->target] = min_node_ptr;
			}
		}
	}

	std::vector<const node *> S;

	const node * path_node_ptr = target_node_ptr;

	if (prev.find(path_node_ptr) == prev.end())
	{
		return;			// Target node is unreachable
	}

	if (path_node_ptr == source_node_ptr)
	{
		return;			// Target node is source node (empty path)
	}

	while (true)
	{
		auto it_find = prev.find(path_node_ptr);

		if (it_find == prev.end())
		{
			break;
		}

		S.push_back(path_node_ptr);

		path_node_ptr = it_find->second;
	}

	return;
}

} // namespace graph
} // namespace containers

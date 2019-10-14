#pragma once

#include <set>
#include <map>

namespace containers {
namespace graph {

/*
function reconstruct_path(cameFrom, current)
total_path : = {current}
	while current in cameFrom.Keys :
		current : = cameFrom[current]
		total_path.prepend(current)
		return total_path
*/

template<typename Graph>
int h(typename Graph::node * node_ptr)
{

}

// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
template<typename Graph>
auto astar(const Graph & g, const typename Graph::node * source_node_ptr, const typename Graph::node * target_node_ptr)
{
	using node = typename Graph::node;
	using edge = typename Graph::edge;

	// The set of discovered nodes that need to be (re-)expanded.
	// Initially, only the start node is known.
	std::set<node *> openSet;
	openSet.insert(source_node_ptr);

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<node *, node *> cameFrom;

	// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
	std::map<node *, int> gScore;
	gScore[source_node_ptr] = 0;

	// Initialize closed set as empty.
	std::set<node *> closedSet;

	// For node n, fScore[n] := gScore[n] + h(n).
	std::map<node *, int> fScore;
	
	fScore[source_node_ptr] = h(source_node_ptr);

/*
	while openSet is not empty
			current : = the node in openSet having the lowest fScore[] value
			if current = goal
				return reconstruct_path(cameFrom, current)

				openSet.Remove(current)
				closedSet.Add(current)
				for each neighbor of current
					if neighbor in closedSet
						continue
						// d(current,neighbor) is the weight of the edge from current to neighbor
						// tentative_gScore is the distance from start to the neighbor through current
						tentative_gScore : = gScore[current] + d(current, neighbor)
						if tentative_gScore < gScore[neighbor]
							// This path to neighbor is better than any previous one. Record it!
							cameFrom[neighbor] : = current
							gScore[neighbor] : = tentative_gScore
							fScore[neighbor] : = gScore[neighbor] + h(neighbor)
							if neighbor not in openSet
								openSet.add(neighbor)

								// Open set is empty but goal was never reached
								return failure
*/
}

} // namespace graph
} // namespace containers
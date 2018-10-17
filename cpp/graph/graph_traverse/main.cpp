#include <iostream>
#include <set>
#include <utility>
#include <exception>
#include <functional>

enum class edge
{
	left,	// left sibling
	right,	// right sibling
	up,		// parent
	down,	// child
	above,	// layer above
	below,	// layer below

	invalid	// invalid node
};

class node
{
public:
	void add(edge e, node * pn)
	{
		edges.emplace(std::make_pair(e, pn));
	}

	void remove(edge e, node * pn)
	{
		edges.erase(std::make_pair(e, pn));
	}

public:
	std::set<std::pair<edge, node *>> edges;
};

enum class action
{
	follow,
	retreat,
	next
};

using callback = action (*)(edge, node *);

void traverse(node * pn, callback c)
{
	auto it = pn->edges.begin();

	while (it != pn->edges.end())
	{
		action d = c(it->first, it->second);

		if (d == action::follow)
		{
			traverse(it->second, c);
			it++;
			continue;
		}

		if (d == action::retreat)
		{
			break;
		}

		if (d == action::next)
		{
			it++;
			continue;
		}

		throw std::exception("Unhandled action.");
	}
}

int main()
{
	node a, b, c, d;

	a.add(edge::down, &b);
	b.add(edge::up, &a);

	a.add(edge::down, &c);
	c.add(edge::up, &a);

	c.add(edge::above, &d);
	d.add(edge::below, &c);

	callback handler = [](edge e, node * pn)
	{
		switch (e)
		{
			case edge::up:
			{
				std::cout << "up";
			}
			break;

			case edge::down:
			{
				std::cout << "down";
				std::cout << " -> follow" << std::endl;
				return action::follow;
			}
			break;

			case edge::above:
			{
				std::cout << "above";
				std::cout << " -> follow" << std::endl;
				return action::follow;
			}
			break;

			case edge::below:
			{
				std::cout << "below";
			}
			break;
		}

		std::cout << " -> next" << std::endl;
		return action::next;
	};

	traverse(&a, handler);

	std::getchar(); return 0;
}
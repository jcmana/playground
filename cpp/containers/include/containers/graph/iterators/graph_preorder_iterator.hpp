#pragma once

#include <algorithm>
#include <vector>

template <typename Graph>
class graph_preorder_iterator
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

	using cursor = typename graph::cursor;

public:
	graph_preorder_iterator();
	graph_preorder_iterator(cursor & c);

	graph_preorder_iterator & operator ++();

	cursor & operator ->();
	cursor & operator  *();

private:
	cursor & top();

	void push(const cursor & c);
	void pop();

private:
	typename std::vector<cursor > cursor_path;

template <typename Graph>
friend bool operator ==(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);

template <typename Graph>
friend bool operator !=(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right);
};

template <typename Graph>
graph_preorder_iterator<Graph>::graph_preorder_iterator()
{
}

template <typename Graph>
graph_preorder_iterator<Graph>::graph_preorder_iterator(cursor & c)
{
	push(c);
}

template <typename Graph>
graph_preorder_iterator<Graph> &
graph_preorder_iterator<Graph>::operator ++()
{
	// Dive:
	if (top())
	{
		auto & current_top = top();
		auto & current_top_outgoing = current_top->target->outgoing;

		// Create `cursor` to the first outgoing `edge` of current `target`
		cursor next(current_top_outgoing.begin(), current_top_outgoing.end(), current_top_outgoing.begin());

		// Push it on top of the stack
		push(next);
	}

	// Emerge until we have valid `cursor` or stack is empty
	while (top() == false)
	{
		// Remove `cursor` from the top of the stack
		pop();

		// THIS IS A BIG PROBLEM COS IT REQUIRES ITERATOR TO KNOW ABOUT ITS END()
		if (cursor_path.size() == 0)
		{
			break;
		}

		// Increment the `cursor` which is currently on top of the stack
		auto & current_top = top();
		++current_top;
	}

	return (*this);
}

template <typename Graph>
typename graph_preorder_iterator<Graph>::cursor &
graph_preorder_iterator<Graph>::operator ->()
{
	return top();
}

template <typename Graph>
typename graph_preorder_iterator<Graph>::cursor &
graph_preorder_iterator<Graph>::operator  *()
{
	return top();
}

template <typename Graph>
typename graph_preorder_iterator<Graph>::cursor &
graph_preorder_iterator<Graph>::top()
{
	return cursor_path.back();
}

template <typename Graph>
void 
graph_preorder_iterator<Graph>::push(typename const graph_preorder_iterator<Graph>::cursor & c)
{
	cursor_path.push_back(c);
}

template <typename Graph>
void 
graph_preorder_iterator<Graph>::pop()
{
	cursor_path.pop_back();
}

template <typename Graph>
bool operator ==(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right)
{
	return (left.cursor_path == right.cursor_path);
}

template <typename Graph>
bool operator !=(const graph_preorder_iterator<Graph> & left, const graph_preorder_iterator<Graph> & right)
{
	return !(left == right);
}

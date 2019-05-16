#pragma once

#include <algorithm>
#include <vector>

template <typename Graph>
class preorder_iterator_c
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

	using cursor = typename graph::cursor;

public:
	preorder_iterator_c();
	preorder_iterator_c(cursor & c);

	preorder_iterator_c & operator ++();

	cursor & operator ->();
	cursor & operator  *();

private:
	void dive();
	void emerge();

	cursor & top();

	void push(const cursor & c);
	void pop();

private:
	typename std::vector<cursor > cursor_path;

template <typename Graph>
friend bool operator ==(const preorder_iterator_c<Graph> & left, const preorder_iterator_c<Graph> & right);

template <typename Graph>
friend bool operator !=(const preorder_iterator_c<Graph> & left, const preorder_iterator_c<Graph> & right);
};

template <typename Graph>
preorder_iterator_c<Graph>::preorder_iterator_c()
{
}

template <typename Graph>
preorder_iterator_c<Graph>::preorder_iterator_c(cursor & c)
{
	push(c);
}

template <typename Graph>
preorder_iterator_c<Graph> & 
preorder_iterator_c<Graph>::operator ++()
{
	// Dive:
	if (top())
	{
		dive();
	}

	// Emerge until we have 
	while (top() == false)
	{
		// Remove `cursor` from the top of the stack
		pop();

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
typename preorder_iterator_c<Graph>::cursor &
preorder_iterator_c<Graph>::operator ->()
{
	return top();
}

template <typename Graph>
typename preorder_iterator_c<Graph>::cursor &
preorder_iterator_c<Graph>::operator  *()
{
	return top();
}

template <typename Graph>
void 
preorder_iterator_c<Graph>::dive()
{
	auto & current_top = top();
	auto & current_top_outgoing = current_top->target->outgoing;

	// Create `cursor` to the first outgoing `edge` of current `target`
	cursor next(current_top_outgoing.begin(), current_top_outgoing.end(), current_top_outgoing.begin());

	// Push it on top of the stack
	push(next);
}

template <typename Graph>
void 
preorder_iterator_c<Graph>::emerge()
{
	// Remove `cursor` from the top of the stack
	pop();

	// 
	if (cursor_path.size() == 0)
	{
		return;
	}

	// Increment the `cursor` which is currently on top of the stack
	auto & current_top = top();
	++current_top;
}

template <typename Graph>
typename preorder_iterator_c<Graph>::cursor & 
preorder_iterator_c<Graph>::top()
{
	return cursor_path.back();
}

template <typename Graph>
void 
preorder_iterator_c<Graph>::push(typename const preorder_iterator_c<Graph>::cursor & c)
{
	cursor_path.push_back(c);
}

template <typename Graph>
void 
preorder_iterator_c<Graph>::pop()
{
	cursor_path.pop_back();
}

template <typename Graph>
bool operator ==(const preorder_iterator_c<Graph> & left, const preorder_iterator_c<Graph> & right)
{
	return (left.cursor_path == right.cursor_path);
}

template <typename Graph>
bool operator !=(const preorder_iterator_c<Graph> & left, const preorder_iterator_c<Graph> & right)
{
	return !(left == right);
}

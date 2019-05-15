#pragma once

#include <vector>
#include <iterator>

template <typename Graph>
class preorder_iterator_c
{
public:
	using graph = Graph;

	using node = typename graph::node;
	using edge = typename graph::edge;

	using cursor = typename graph::cursor;

public:
	preorder_iterator_c(const cursor & c)
	{
		push(c);
	}

	preorder_iterator_c & operator ++()
	{
		// Remove exhausted cursors from path
		while (cursor_path.back() == false)
		{
			pop();
		}

		cursor cursor_current = cursor_path.back();

		if (cursor_current)
		{
			++cursor_current;
			push(cursor_current);

			cursor cursor_next(cursor_current->target->outgoing);
			push(cursor_next);
		}

		return *this;
	}

private:
	void push(const cursor & c)
	{
		cursor_path.push_back(c);
	}

	void pop()
	{
		cursor_path.pop_back();
	}

private:
	typename std::vector<cursor> cursor_path;
};
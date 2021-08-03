#pragma once

#include <cassert>
#include <vector>

namespace containers::graph::experimental
{

/// \brief		Dual-stack graph cursor.
///
/// Dual-stack automata with specific set of operations, binding both stacks together in a way
/// compatible with directed acyclic graph traversal.
///
/// First stack (down) represents searching down the graph through each possible edge.
///
/// Second stack (path) represents current position as unique path through the graph.
template<typename T>
class path_cursor
{
public:
	/// \brief		Default constructur, intializes empty cursor.
	path_cursor() :
		m_stack_down(),
		m_stack_path()
	{
	}

	path_cursor(std::vector<T> expansion_elements) :
		m_stack_down(std::move(expansion_elements)),
		m_stack_path()
	{
	}

	/// \brief		Removes top element from both stacks.
	///
	/// This represents leaving current subgraph of the graph upwards.
	void pop()
	{
		m_stack_down.pop_back();
		m_stack_path.pop_back();
	}

	/// \brief		Push expansion of `path_element` onto stacks.
	///
	/// This represents operation of moving down the graph through the `path_element` while planning to
	/// visit all `expansion_elements` next.
	void push(T path_element, const std::vector<T> & expansion_elements)
	{
		m_stack_down.insert(m_stack_down.end(), expansion_elements.begin(), expansion_elements.end());
		m_stack_path.push_back(path_element);
	}

	/// \brief		Top element from expansion stack.
	const T & top() const
	{
		return m_stack_down.back();
	}

	/// \brief			Expansion stack is empty.
	bool empty() const
	{
		return m_stack_down.empty();
	}

	/// \brief			Expansion and path stack last nodes match.
	bool match() const
	{
		if (m_stack_down.empty())
		{
			return false;
		}

		if (m_stack_path.empty())
		{
			return false;
		}

		return (m_stack_down.back() == m_stack_path.back());
	}

	/// \brief			Path stack size.
	std::size_t size() const
	{
		return m_stack_path.size();
	}

	bool operator ==(const path_cursor & other) const
	{
		if (m_stack_down.size() != other.m_stack_down.size())
		{
			return false;
		}

		if (m_stack_path.size() != other.m_stack_path.size())
		{
			return false;
		}

		if (m_stack_down.empty() == other.m_stack_down.empty())
		{
			return true;
		}

		if (m_stack_path.empty() == other.m_stack_path.empty())
		{
			return true;
		}

		// Full comparison of down stacks:
		{
			auto it_down_this = m_stack_down.begin();
			auto it_down_that = other.m_stack_down.begin();

			for (; it_down_this != m_stack_down.end(); ++it_down_this, ++it_down_that)
			{
				if (*it_down_this != *it_down_that)
				{
					return false;
				}
			}
		}

		// Full comparison of path stacks:
		{
			auto it_path_this = m_stack_path.begin();
			auto it_path_that = other.m_stack_path.begin();

			for (; it_path_this != m_stack_path.end(); ++it_path_this, ++it_path_that)
			{
				if (*it_path_this != *it_path_that)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator !=(const path_cursor & other) const
	{
		return !((*this) == other);
	}

	const std::vector<T> & operator  *() const
	{
		return m_stack_path;
	}

	const std::vector<T> * operator ->() const
	{
		return &m_stack_path;
	}

private:
	std::vector<T> m_stack_down;
	std::vector<T> m_stack_path;
};

} // namespace containers::graph::experimental


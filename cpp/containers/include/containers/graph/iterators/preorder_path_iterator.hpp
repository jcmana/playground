#pragma once

#include "../cursors/path_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
struct preorder_path_iterator : path_cursor<G>
{
	using path_cursor<G>::path_cursor;

	/// \brief		Finds next path in postorder traversal.
	preorder_path_iterator & operator ++()
	{
		const auto cursor_depth = depth();

		for (std::size_t n = 0; n < cursor_depth && match(); ++n)
		{
			consume();
		}

		if (empty() == false)
		{
			expand();
		}

		return (*this);
	}

	/// \brief		Skips entire subtree.
	void skip()
	{
		const auto cursor_depth = depth();

		for (std::size_t n = 0; n < cursor_depth && !match(); ++n)
		{
			path_cursor<G>::skip();
		}
	}
};

} // namespace graph
} // namespace containers

#pragma once

//#include <vector>
#include "../cursors/preorder_path_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
struct preorder_path_iterator : preorder_path_cursor<G>
{
	using preorder_path_cursor<G>::preorder_path_cursor;

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
			preorder_path_cursor<G>::skip();
		}
	}
};

} // namespace graph
} // namespace containers

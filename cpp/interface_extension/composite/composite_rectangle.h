#pragma once

#include "indexable_impl.h"
#include "element_impl.h"
#include "rectangle_impl.h"

class composite_rectangle :
	public indexable_impl,
	public element_impl,
	public rectangle_impl
{
public:
	class factory
	{
	public:
		static std::shared_ptr<composite_rectangle> make_composite(int x, int y, int width, int height);
	};

public:
	composite_rectangle(
		indexable_impl && constructed_indexable_impl,
		element_impl && constructed_element_impl,
		rectangle_impl && constructed_rectangle_impl
	);
};
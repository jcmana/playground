#include "composite_rectangle.h"

std::shared_ptr<composite_rectangle> composite_rectangle::factory::make_composite(int x, int y, int width, int height)
{
	// indexable_impl instantiation:
	int boundary = (x + y + width + height);
	indexable_impl && constructed_indexable_impl(boundary);

	// element_impl instantiation:
	element_impl constructed_element_impl(3 /* element_type::rectangle */);

	// rectangle_impl instantiation:
	rectangle_impl constructed_rectangle_impl(x, y, width, height);

	// composite instantiation:
	return std::make_shared<composite_rectangle>(
		std::move(constructed_indexable_impl),
		std::move(constructed_element_impl),
		std::move(constructed_rectangle_impl)
	);
}

composite_rectangle::composite_rectangle(
	indexable_impl && constructed_indexable_impl,
	element_impl && constructed_element_impl,
	rectangle_impl && constructed_rectangle_impl
) :
	indexable_impl(constructed_indexable_impl),
	element_impl(constructed_element_impl),
	rectangle_impl(constructed_rectangle_impl)
{
}

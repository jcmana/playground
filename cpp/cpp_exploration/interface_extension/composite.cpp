#include "composite.h"

std::shared_ptr<composite> composite::factory::make_composite(int x, int y, int width, int height)
{
	// indexable_impl instantiation
	int boundary = (x + y + width + height);
	indexable_impl constructed_indexable_impl(boundary);

	return std::make_shared<composite>(std::move(constructed_indexable_impl));
}

composite::composite(indexable_impl && constructed_indexable_impl) :
	indexable_impl(constructed_indexable_impl),
	element_impl(0),
	rectangle_impl(1, 2, 3, 4)
{
}

#include <iostream>
#include <memory>

#include "indexable_impl.h"
#include "element_impl.h"
#include "rectangle_impl.h"

#include "composite.h"

int main()
{
	// building blocks
	std::shared_ptr<indexable> i = std::make_shared<indexable_impl>(6);
	std::shared_ptr<element> e = std::make_shared<element_impl>(0);
	std::shared_ptr<rectangle> r = std::make_shared<rectangle_impl>(5, 5, 20, 10);

	i->boundary();
	e->type();
	r->height();

	// composite class
	auto comp = composite::factory::make_composite(5, 5, 20, 10);

	// side-casting
	auto indexable_comp = std::static_pointer_cast<indexable>(comp);
	auto element_comp = std::static_pointer_cast<element>(comp);
	auto rectangle_comp = std::static_pointer_cast<rectangle>(comp);

	auto boundary = comp->boundary();

	// illegal side-casting attempt
	auto illegal_comp = std::dynamic_pointer_cast<composite>(i);

	// static_cast will succeed
	// dynamic_cast will return nullptr
	if (illegal_comp)
	{
		illegal_comp->width();
	}

	return 0;
}
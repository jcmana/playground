#include <iostream>
#include <memory>

#include "rectangle_impl.h"

int main()
{
	// with shared pointers:
	{
		std::shared_ptr<indexable> sp_indexable = std::make_shared<rectangle_impl>(5, 5, 20, 15);

		std::cout << "indexable::boundary() = " << sp_indexable->boundary() << std::endl;

		auto sp_indexable_to_element = std::static_pointer_cast<element>(sp_indexable);
		std::cout << "element::type() = " << sp_indexable_to_element->type() << std::endl;

		auto sp_indexable_to_rectangle = std::static_pointer_cast<rectangle>(sp_indexable);
		std::cout << "rectangle::width() = " << sp_indexable_to_rectangle->width() << std::endl;
	}

	std::getchar();	return 0;
}
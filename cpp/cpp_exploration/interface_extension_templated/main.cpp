#include <iostream>
#include <memory>

#include "indexable_impl.h"
#include "element_impl.h"
#include "rectangle_impl.h"

int main()
{
	// with shared pointers:
	{
		std::shared_ptr<rectangle> sp_rectangle = std::make_shared<rectangle_impl>(5, 5, 20, 15);

		auto sp_rectangle_to_indexable = std::static_pointer_cast<indexable>(sp_rectangle);
		std::cout << "indexable::boundary() = " << sp_rectangle_to_indexable->boundary() << std::endl;

		auto sp_rectangle_to_element = std::static_pointer_cast<element>(sp_rectangle);
		std::cout << "element::type() = " << sp_rectangle_to_element->type() << std::endl;
	}

	std::getchar();	return 0;
}
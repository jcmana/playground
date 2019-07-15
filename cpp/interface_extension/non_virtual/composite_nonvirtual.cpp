#include "rectangle.h"

int main()
{
	element_base * element_ptr = new rectangle(0, 0, 10, 20);

	rectangle * rectangle_ptr = static_cast<rectangle *>(element_ptr);

	return 0;
}
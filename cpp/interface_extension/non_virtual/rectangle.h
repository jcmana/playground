#pragma once

#include "element_base.h"
#include "indexable_base.h"
#include "rectangle_base.h"

class rectangle :
	public element_base,
	public indexable_base,
	public rectangle_base
{
public:
	rectangle(int x, int y, int width, int height) :
		element_base(0),
		indexable_base(x + y + width + height),
		rectangle_base(x, y, width, height)
	{
	}
};
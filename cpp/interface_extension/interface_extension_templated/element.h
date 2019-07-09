#pragma once

#include "indexable.h"

class element :
	public indexable
{
public:
	virtual unsigned int type() const = 0;
};
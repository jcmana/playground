#pragma once

#include <memory>

#include "indexable.h"
#include "element.h"

class rectangle
{
public:
	virtual int  width() = 0;
	virtual int  height() = 0;
};
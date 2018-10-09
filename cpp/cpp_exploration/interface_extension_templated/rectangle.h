#pragma once

#include "element.h"

class rectangle :
	public element
{
public:
	virtual int width() const = 0;
	virtual int height() const = 0;
};
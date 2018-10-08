#pragma once

#include "element.h"

class element_impl :
	public element
{
public:
	element_impl(unsigned int type);

public:
	virtual unsigned int type() override;

private:
	unsigned int m_type;
};
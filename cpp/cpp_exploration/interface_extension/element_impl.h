#pragma once

#include "element.h"

class element_impl :
	public element
{
public:
	element_impl(unsigned int type);
	element_impl(const element_impl & other) = default;
	element_impl(element_impl && other) noexcept = default;

public:
	virtual unsigned int type() override;

private:
	unsigned int m_type;
};
#pragma once

#include "indexable_impl.h"
#include "element.h"

template <typename Interface>
class element_impl :
	public indexable_impl<Interface>
{
public:
	element_impl(int boundary, unsigned int type);
	element_impl(const element_impl & other) = default;
	element_impl(element_impl && other) noexcept = default;

public:
	virtual unsigned int type() const override;

private:
	unsigned int m_type;
};

template <typename Interface>
element_impl<Interface>::element_impl(int boundary, unsigned int type) :
	indexable_impl(boundary),
	m_type(type)
{
}

template <typename Interface>
unsigned int element_impl<Interface>::type() const
{
	return m_type;
}

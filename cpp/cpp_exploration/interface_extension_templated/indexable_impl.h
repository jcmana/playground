#pragma once

#include "indexable.h"

template <typename Interface>
class indexable_impl : public Interface
{
public:
	indexable_impl(int boundary);
	indexable_impl(const indexable_impl & other) = default;
	indexable_impl(indexable_impl && other) noexcept = default;

public:
	virtual int boundary() const override;

private:
	int m_boundary = 0;
};


template <typename Interface>
indexable_impl<Interface>::indexable_impl(int boundary) :
	m_boundary(boundary)
{
}

template <typename Interface>
int indexable_impl<Interface>::boundary() const
{
	return m_boundary;
}

#pragma once

#include <string>

#include "indexable.h"

class indexable_impl :
	public indexable
{
public:
	indexable_impl(int boundary);
	indexable_impl(const indexable_impl & other) = default;
	indexable_impl(indexable_impl && other) noexcept = default;

public:
	virtual int boundary() override;

private:
	int m_boundary;
};
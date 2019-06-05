#pragma once

#include <memory>

namespace pmr {
namespace containers {

template <typename T>
class forward_iterator_facade
{
public:
	virtual ~forward_iterator_facade() = default;

	/// \brief		Move to the next element.
	virtual void next() = 0;
	
	/// \brief		Access current element.
	virtual T & read() = 0;
	
	/// \brief		Compare for equality.
	virtual bool equal(const forward_iterator_facade<T> & other) const = 0;

	/// \brief		Obtain iterator copy.
	virtual std::unique_ptr<forward_iterator_facade<T>> copy() const = 0;
};

} // namespace containers
} // namespace pmr
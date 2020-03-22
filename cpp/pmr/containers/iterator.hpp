#pragma once

#include <memory>
#include <utility>

namespace pmr {

template<typename T>
class iterator
{
public:
	class facade
	{
	public:
		virtual ~facade() = default;

		virtual T & value() = 0;
		virtual bool equal(const facade & other) const = 0;
		virtual std::unique_ptr<facade> copy() const = 0;
	};

	// Standard library compliance aliases:
	using value_type = T;
	using pointer = T *;
	using reference = T &;

public:
	iterator() = default;
	iterator(std::unique_ptr<facade> up_facade) :
		m_up_facade(std::move(up_facade))
	{
	}
	iterator(const iterator & other) :
		m_up_facade(other.m_up_facade->copy())
	{
	}

	iterator(iterator && other) = default;

	template<typename TT>
	friend bool operator ==(const iterator<T>& lhs, const iterator<T> & rhs)
	{
		return (*lhs.m_up_facade).equal(*rhs.m_up_facade);
	}

	template<typename TT>
	friend bool operator !=(const iterator<T> & lhs, const iterator<T> & rhs)
	{
	}

	T * operator ->()
	{
		return &(m_up_facade->value());
	}

	T & operator  *()
	{
		return (m_up_facade->value());
	}

protected:
	std::unique_ptr<facade> m_up_facade;
};

} // namepsace pmr

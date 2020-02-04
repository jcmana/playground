#pragma once

#include <mutex>

#include "../../link/link/link_element.hpp"

template<typename T>
class atomic_callback;

template<typename T>
class atomic_callback_guard : private link_element
{
public:
	friend class atomic_callback<T>;

public:
	atomic_callback_guard()
    {
    }

	explicit atomic_callback_guard(atomic_callback<T> & callback_ref) :
		m_lock(callback_ref.m_mutex, std::defer_lock),
		link_element(&callback_ref)
	{
	}

    atomic_callback_guard(atomic_callback_guard && other) :
        m_lock(std::move(other.m_lock)),
        link_element(std::move(other))
    {
    }

	~atomic_callback_guard()
	{
        if (link_element::is_linked())
        {
            // JMTODO: fix the race condition here

		    std::unique_lock<std::unique_lock<std::mutex>> lock(m_lock);
            link_element::release();
        }
	}

private:
	mutable std::unique_lock<std::mutex> m_lock;
};
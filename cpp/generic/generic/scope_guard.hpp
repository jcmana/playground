#pragma once

#include <atomic>

namespace generic {
	
/// <summary>Executes functor on scope destruction. Simplest of all the scope guards.</summary>
class scope_guard
{
public:
	using functor = void(*)();

public:
	/// <summary>Constructor sets up activated scope guard with end-of-scope action.</summary>
	/// <param id="action">Functor to be executed (expected operator ()).</param>
	scope_guard(functor action)
	{
		m_action = action;
		m_active = true;
	}

	scope_guard(scope_guard && other)
	{
		std::swap(m_active, other.m_active);
		std::swap(m_action, other.m_action);
	}

	scope_guard & operator =(const scope_guard & other) = delete;
	scope_guard & operator ==(scope_guard && other) = delete;

	/// <summary>Destructor calls requested action.</summary>
	~scope_guard()
	{
		// Check if scope guard is activated
		if (m_active == false)
		{
			return;
		}

		m_action();
		m_active = false;
	}

	/// <summary>Functor won't be executed.</summary>
	void remove()
	{
		m_active = false;
	}

private:
	bool m_active = false;
	functor m_action = nullptr;
};

/// <summary>Creates new scope guard with given functor.</summary>
/// <param id="action">Functor to be executed (expected operator ()).</param>
/// <returns>Activated scope_guard instance.</returns>
scope_guard make_scope_guard(scope_guard::functor action)
{
	return scope_guard(action);
}

} // namespace generic

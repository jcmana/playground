#pragma once

#include <memory>

namespace generic {


/// <summary>Scope guard callback function signature.</summary>
using scope_guard_functor = void(*)();

/// <summary>Executes functor on scope destruction. Simplest of all the scope guards.</summary>
class scope_guard
{
public:
	explicit scope_guard(scope_guard_functor action)
	{
		m_action = action;
	}

	~scope_guard()
	{
		if (m_action != nullptr)
		{
			m_action();
		}
	}

	/// <summary>Functor won't be executed.</summary>
	void remove()
	{
		m_action = nullptr;
	}

private:
	scope_guard_functor m_action;
};

/// <summary>Creates new scope guard with given functor.</summary>
/// <param id="action">Action to be executed.</param>
/// <returns>Scope guard instance.</returns>
std::shared_ptr<scope_guard> make_scope_guard(scope_guard_functor action)
{
	return std::make_shared<scope_guard>(action);
}


} // namespace generic

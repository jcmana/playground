#include <utility>

#include "scope_guard.h"

namespace generic {

scope_guard::scope_guard(functor action)
{
	m_action = action;
	m_active = true;
}

scope_guard::scope_guard(scope_guard && other)
{
	std::swap(m_active, other.m_active);
	std::swap(m_action, other.m_action);
}

scope_guard::~scope_guard()
{
	// Check if scope guard is activated
	if (m_active == false)
	{
		return;
	}

	m_action();
	m_active = false;
}

void scope_guard::remove()
{
	m_active = false;
}

scope_guard make_scope_guard(scope_guard::functor action)
{
	return scope_guard(action);
}

} // namespace generic
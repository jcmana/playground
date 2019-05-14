#pragma once

namespace generic {
	
/// <summary>Executes functor on scope destruction. Simplest of all the scope guards.</summary>
class scope_guard
{
public:
	using functor = void(*)();

public:
	scope_guard(functor action);
	scope_guard(scope_guard && other);

	~scope_guard();

	scope_guard & operator =(const scope_guard & other) = delete;
	scope_guard & operator =(scope_guard && other) = delete;

	void remove();

private:
	bool m_active = false;
	functor m_action = nullptr;
};

scope_guard make_scope_guard(scope_guard::functor action);

} // namespace generic

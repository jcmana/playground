#pragma once


#include "event_distributor_intf.h"
#include "registration_holder.h"

// KTTODO	- Optimize - you should block entire event distributor during invoking callback.
//			- Local copy of vector + resolve unregistering using second mutex.

namespace ed
{

template<class callback_t>
struct impl:
	event_distributor_intf<callback_t>
{
private:
	// Fake context for registration holder:
	using fake_ctx_t = uint32_t;
	const fake_ctx_t FAKE_CTX = 0;

public:
	virtual std::shared_ptr<registrator_intf> register_callback(callback_t& cbk) override;

	virtual void notify_all(std::function<void(callback_t*)> f) override;

private:
	detail::registration_holder<callback_t, fake_ctx_t> m_reg_holder;
};


template<class callback_t, typename context_t>
struct impl_ctx:
	event_distributor_ctx_intf<callback_t, context_t>
{
	virtual std::shared_ptr<registrator_intf> register_callback(callback_t& cbk, const context_t& ctx) override;

	virtual void notify_all(std::function<void(callback_t*, const context_t&)> f, const context_t& ctx) override;

private:
	detail::registration_holder<callback_t, context_t> m_reg_holder;
};


}

#include "event_distributor_impl.inl"


template<class callback_t>
std::shared_ptr<registrator_intf> ed::impl<callback_t>::register_callback(callback_t& cbk)
{
	return m_reg_holder.register_callback(cbk, FAKE_CTX);
}


template<class callback_t>
void ed::impl<callback_t>::notify_all(std::function<void(callback_t*)> f)
{
	std::lock_guard<std::mutex> l(m_reg_holder.m_storage_mutex);

	// KTTODO - create local copy and unlock l. Take care for unregistrations.
	std::vector<callback_t*>& regs = m_reg_holder.m_storage[FAKE_CTX];

	// Invoke callbacks:
	for (auto r : regs)
	{
		f(r);
	}
}


template<class callback_t, typename context_t>
std::shared_ptr<registrator_intf> ed::impl_ctx<callback_t, context_t>::register_callback(callback_t& cbk, const context_t& ctx)
{
	return m_reg_holder.register_callback(cbk, ctx);
}


template<class callback_t, typename context_t>
void ed::impl_ctx<callback_t, context_t>::notify_all(std::function<void(callback_t*, const context_t&)> f, const context_t& ctx)
{
	std::lock_guard<std::mutex> l(m_reg_holder.m_storage_mutex);

	// KTTODO - create local copy and unlock l. Take care for unregistrations.
	std::vector<callback_t*>& regs = m_reg_holder.m_storage[ctx];

	for (auto r : regs)
	{
		f(r, ctx);
	}
}

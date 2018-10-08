template<class callback_t, typename context_t>
ed::detail::registration_holder<callback_t, context_t>::reg_impl::reg_impl(registration_holder<callback_t, context_t>& ed, callback_t& cbk, const context_t& ctx) :
	ed(&ed),
	cbk(&cbk),
	ctx(ctx)
{}


template<class callback_t, typename context_t>
ed::detail::registration_holder<callback_t, context_t>::reg_impl::~reg_impl()
{
	std::lock_guard<std::mutex> l(ed->m_storage_mutex);

	// Find and erase handler:
	auto it = std::find(ed->m_storage[ctx].begin(), ed->m_storage[ctx].end(), cbk);
	ed->m_storage[ctx].erase(it);
}


template<class callback_t, typename context_t>
std::shared_ptr<registrator_intf> ed::detail::registration_holder<callback_t, context_t>::register_callback(callback_t& cbk, const context_t& ctx)
{
	// Register to distributor:
	std::lock_guard<std::mutex> l(m_storage_mutex);
	m_storage[ctx].push_back(&cbk);

	// Create registration object:
	std::shared_ptr<registrator_intf> reg;

	try
	{
		reg = std::make_shared<reg_impl>(*this, cbk, ctx);
	}
	catch (...)
	{
		// Failed to create registration object - unregister manualy and pass exception:
		m_storage[ctx].pop_back();
		std::rethrow_exception(std::current_exception());
	}

	// Success:
	return reg;
}

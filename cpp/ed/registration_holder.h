#include <mutex>
#include <vector>
#include <unordered_map>


namespace ed
{

namespace detail
{

template<class callback_t, typename context_t>
struct registration_holder
{
	// Registration object:
	struct reg_impl:
		registrator_intf
	{
		reg_impl(registration_holder& ed, callback_t& cbk, const context_t& ctx);
		~reg_impl();

	private:
		registration_holder* ed;
		callback_t* cbk;
		context_t ctx;
	};

	std::shared_ptr<registrator_intf> register_callback(callback_t& cbk, const context_t& ctx);

	std::mutex m_storage_mutex;

	// Registrations are stored as vector under the same context:
	std::unordered_map<context_t, std::vector<callback_t*>> m_storage;
};


}


}


#include "registration_holder.inl"

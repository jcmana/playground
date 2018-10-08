#pragma once


#include <memory>
#include <functional>
#include "registrator_intf.h"


/*!	\brief	Interface to event distributor without context.

	\code{.cpp}
	// Definition of class which receiving callbacks by 'some_callback' interface:
	class some_class: public some_callback
	{
		void on_callback(int val) override { do_something(); }
	};

	// Definition of callback handling class instance:
	some_class* pTest; 

	// Definition of event distributor for 'some_callback' interface:
	event_distributor_intf<some_callback>* pEd;
	\endcode
*/
template<class callback_t>
struct event_distributor_intf
{
	/*!	\brief		Register callback to distributor.
		\param[in]	cbk	Reference to callback handler.
		\return		Registration object. Destruction of this object causes unregister.
		\code{.cpp}
		auto reg = pEd->register_callback(*pTest);
		// Now you get all callbacks from event distributor.
		reg.reset();
		// Now you are unregistered.
		\endcode
	*/
	virtual std::shared_ptr<registrator_intf> register_callback(callback_t& cbk) = 0;

	/*!	\brief		Notify all registered callback handlers.
		\param[in]	f	Callback invoker.
		\code{.cpp}
		// - Notify all registered listeners with some value:
		int val = compute_value();
		pEd->notify_all([val](some_callback* p) { p->on_callback(val); });
		\endcode
	*/
	virtual void notify_all(std::function<void(callback_t*)> f) = 0;
};


/*!	\brief		Interface to event distributor with context.

	\code{.cpp}
	// Definition of class which receiving callbacks by 'some_callback' interface:
	class some_class: public some_callback
	{
		void on_callback(int val, const std::string& ctx) override { do_something(); }
	};

	// Definition of callback handling class instance:
	some_class* pTest; 

	// Definition of event distributor for 'some_callback' interface witch std::string context:
	event_distributor_intf<some_callback, std::string>* pEd;
	\endcode
*/
template<class callback_t, typename context_t>
struct event_distributor_ctx_intf
{
	/*!	\brief		Register callback to distributor under some context.
		\param[in]	cbk	Reference to callback handler.
		\param[in]	ctx	Context.
		\return		Registration object. Destruction of this object causes unregister.
		\code{.cpp}
		auto reg = pEd->register_callback(*pTest, "1");
		// Now you get all callbacks from event distributor with context '"1"'.
		reg.reset();
		// Now you are unregistered.
		\endcode
	*/
	virtual std::shared_ptr<registrator_intf> register_callback(callback_t& cbk, const context_t& ctx) = 0;

	/*!	\brief		Notify all registered callback handlers under some context.
		\param[in]	f	Callback invoker.
		\code{.cpp}
		// - Notify all registered listeners with some value under context:
		some_class test1;
		some_class test2;
		
		pEd->register_callback(test1, "1"); // register for ctx "1"
		pEd->register_callback(test2, "2"); // register for ctx "2"
		
		int val = compute_value();

		// Notify only listeners registered under ctx "1"
		pEd->notify_all([val](some_callback* p, const std::string& c) { p->on_callback(val, c); }, "1");
		\endcode
	*/
	virtual void notify_all(std::function<void(callback_t*, const context_t&)>, const context_t&) = 0;
};

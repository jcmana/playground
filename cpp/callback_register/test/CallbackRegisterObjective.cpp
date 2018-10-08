#include <chrono>
#include <thread>

using callback_cookie = int;

template<
struct callback_register
{
	callback_cookie Register()
	{
		return 5;
	};
};

struct ShieldsActivityWatcherHandler
{
	virtual void OnActivityStart() = 0;
	virtual void OnActivityEnd() = 0;
};

struct ShieldsActivityWatcherRegistrator
{

};

struct ShieldsActivityWatcher : public callback_register
{
	callback_cookie * Register(ShieldsActivityWatcherHandler * handler)
	{
		return callback_register::Register(ShieldsActivityWatcherHandler);
	}

	void Watch()
	{
		while (true)
		{
			m_handler_list[0]->OnActivityStart();

			std::this_thread::sleep_for(std::chrono::seconds(10));

			m_handler_list[0]->OnActivityEnd();
		}
	}
};

struct AntivirusTray : public ShieldsActivityWatcherHandler
{
	virtual void OnActivityStart()
	{
		// handle stuff
	}

	virtual void OnActivityEnd()
	{
		// handle stuff
	}
};

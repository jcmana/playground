template<typename ...TCallbackParamPack>
functional<TCallbackParamPack...>::InvalidCallbackID::InvalidCallbackID(const std::string & what_arg) :
	std::exception(what_arg)
{
}

template<typename ...TCallbackParamPack>
functional<TCallbackParamPack...>::InvalidCallbackID::InvalidCallbackID(const char * what_arg) :
	std::exception(what_arg)
{
}

template<typename ...TCallbackParamPack>
functional<TCallbackParamPack...>::functional()
{
	m_callbackIDCounter = 0;
}

template<typename ...TCallbackParamPack>
typename functional<TCallbackParamPack...>::TCallbackID
functional<TCallbackParamPack...>::Register(
	typename functional<TCallbackParamPack...>::TCallback callback)
{
	TCallbackID callbackIDCounter = m_callbackIDCounter;

	{
		std::lock_guard<std::mutex> lock(m_callbackRegisterMutex);

		// Store callback in the register under unique ID
		m_callbackRegister[callbackIDCounter] = callback;

		// Increment callback ID counter
		++m_callbackIDCounter;
	}

	// Return callback unique ID
	return callbackIDCounter;
}

template<typename ...TCallbackParamPack>
void
functional<TCallbackParamPack...>::Unregister(
	typename functional<TCallbackParamPack...>::TCallbackID callbackID)
{
	std::size_t removedCallbackCount = 0;

	{
		std::lock_guard<std::mutex> lock(m_callbackRegisterMutex);

		// Remove callback from the register
		removedCallbackCount = m_callbackRegister.erase(callbackID);
	}

	// Check if callback was removed
	if (removedCallbackCount != 1)
	{
		throw InvalidCallbackID("Invalid callback ID received. No callbacks were unregistered.");
	}
}

template<typename ...TCallbackParamPack>
void
functional<TCallbackParamPack...>::Clear()
{
	std::lock_guard<std::mutex> lock(m_callbackRegisterMutex);

	// Remove all callbacks from the register
	m_callbackRegister.clear();
}

template<typename ...TCallbackParamPack>
void
functional<TCallbackParamPack...>::Execute(
	TCallbackID callbackID,
	TCallbackParamPack... callbackParamsPack)
{
	std::lock_guard<std::mutex> lock(m_callbackRegisterMutex);

	m_callbackRegister[callbackID](callbackParamsPack...);
}

template<typename ...TCallbackParamPack>
void
functional<TCallbackParamPack...>::ExecuteAll(
	TCallbackParamPack... callbackParamsPack)
{
	std::lock_guard<std::mutex> lock(m_callbackRegisterMutex);

	for (auto & callback : m_callbackRegister)
	{
		callback.second(callbackParamsPack...);
	}
}

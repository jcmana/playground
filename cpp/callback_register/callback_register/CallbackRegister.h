#pragma once

#include <cstddef>
#include <map>
#include <mutex>
#include <exception>
#include <string>

namespace asw {
namespace callback_register {


///<summary>Template class implementing generic multiple-parameter function callback register.</summary>
///<remarks>
template <typename ...TCallbackParamPack>
class functional
{
public:
	///<summary>Callback identification type. Used for unregistering existing callback.</summary>
	using TCallbackID = int;

	///<summary>Callback function signature. Callback has always void return type.</summary>
	using TCallback = void(*)(TCallbackParamPack...);

	///<summary>Exception. Reports that invalid callback ID parameter was received.</summary>
	class InvalidCallbackID : public std::exception
	{
	public:
		explicit InvalidCallbackID(const std::string & what_arg);
		explicit InvalidCallbackID(const char * what_arg);
	};

public:
	///<summary>Default constructor.</summary>
	functional();

	///<summary>Registers new callback.</summary>
	///<param id="callback">Callback function pointer.</param>
	TCallbackID Register(TCallback callback);

	///<summary>Unregisters specific callback.</summary>
	///<param id="callbackID">Callback identification (from Register() call).</param>
	///<remarks>May throw <i>UnknownCallbackID</i>.</remarks>
	void Unregister(TCallbackID callbackID);

	///<summary>Unregisters all callbacks.</summary>
	void Clear();

	///<summary>Executes specific callback.</summary>
	///<param id"callbackID">Callback identification (from Register() call).</param>
	///<param id="callbackParamsPack">Function parameters to pass into callback functions.</param>
	void Execute(TCallbackID callbackID, TCallbackParamPack... callbackParamsPack);

	///<summary>Executes all currently registered callbacks in order they were registered.</summary>
	///<param id="callbackParamsPack">Function parameters to pass into callback functions.</param>
	void ExecuteAll(TCallbackParamPack... callbackParamsPack);

private:
	TCallbackID							m_callbackIDCounter;
	std::map<TCallbackID, TCallback>	m_callbackRegister;
	std::mutex							m_callbackRegisterMutex;
};

#include "CallbackRegister.cpp"


} // namespace callback_register
} // namespace asw
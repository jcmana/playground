#pragma once

/*!
	JMTODO: doplnit zmrde
*/
class IWindowMessageDistributor
{
	public:
		/*!
			JMTODO: doplnit zmrde
		*/
		virtual void RegisterHandler() = 0;

		/*!
			JMTODO: doplnit zmrde
		*/
		virtual void UnregisterHandler() = 0;
};
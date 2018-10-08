#pragma once

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

class IDesktopWindowMessageHandler
{
public:
	/*!
		\brief	Callback method when window message distributor receives window message.
	*/
	virtual bool OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

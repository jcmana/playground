#pragma once

#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

class IDesktopWindowCallback
{
public:
	/// <summary>Callback for when window receives message.</summary>
	/// <remarks>Must not be called manually.</remarks>
	virtual bool OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
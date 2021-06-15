#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

class IDesktopWindowClass
{
public:
	///	<summary>Registers new window class.</summary>
	/// <remarks>Before registering class you need to set desired values using interface Set* methods.</remarks>
	virtual bool Register() = 0;
	virtual bool Unregister() = 0;

	virtual bool SetStyle(UINT uStyle) = 0;
	virtual bool SetClassExtra(int sbClsExtra) = 0;
	virtual bool SetInstance(HINSTANCE hInstance) = 0;
	virtual bool SetIcon(HICON hIcon) = 0;
	virtual bool SetIconSmall(HICON hIconSmall) = 0;
	virtual bool SetCursor(HCURSOR hCursor) = 0;
	virtual bool SetBackgroundBrush(HBRUSH hBackgroundBrush) = 0;
	virtual bool SetMenuName(const std::wstring & wsMenuName) = 0;
	virtual bool SetClassName(const std::wstring & wsClassName) = 0;
};

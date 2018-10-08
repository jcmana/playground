#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

namespace asw {
namespace winapi {


class IDesktopWindow
{
public:
	virtual bool Create() = 0;
	virtual bool Destroy() = 0;

	virtual bool SetClassName(const std::wstring & wsClassName) = 0;
	virtual bool SetWindowName(const std::wstring & wsWindowName) = 0;
	virtual bool SetStyle(DWORD dwStyle) = 0;
	virtual bool SetExStyle(DWORD dwExStyle) = 0;
	virtual bool SetX(int x) = 0;
	virtual bool SetY(int y) = 0;
	virtual bool SetWidth(int nWidth) = 0;
	virtual bool SetHeight(int nHeight) = 0;
	virtual bool SetParent(HWND hWnd) = 0;
	virtual bool SetMenu(HMENU hMenu) = 0;
	virtual bool SetInstance(HINSTANCE hInstance) = 0;

	virtual HWND GetSafeHwnd() = 0;
	virtual HWND GetParentHwnd() = 0;
};


} // namespace winapi
} // namespace asw

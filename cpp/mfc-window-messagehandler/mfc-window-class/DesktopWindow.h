#pragma once

#include <string>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

#include "DesktopWindow_intf.h"
#include "DesktopWindowClass_intf.h"
#include "DesktopWindowCallback_intf.h"

namespace asw {
namespace winapi {


class CDesktopWindow :
	public IDesktopWindow,
	public IDesktopWindowCallback
{
public:
	class Factory
	{
	public:
		virtual std::shared_ptr<IDesktopWindow> CreateDesktopWindow() const;
	};

	CDesktopWindow();
	~CDesktopWindow();


// IDesktopWindow interface methods implementation.
// This is final implmentation of methods and cannot be overriden in specializad class.
public:
	bool Create() override final;
	bool Destroy() override final;

	bool SetClassName(const std::wstring & wsClassName) override final;
	bool SetWindowName(const std::wstring & wsWindowName) override final;
	bool SetStyle(DWORD dwStyle) override final;
	bool SetExStyle(DWORD dwExStyle) override final;
	bool SetX(int x) override final;
	bool SetY(int y) override final;
	bool SetWidth(int nWidth) override final;
	bool SetHeight(int nHeight) override final;
	bool SetParent(HWND hWnd) override final;
	bool SetMenu(HMENU hMenu) override final;
	bool SetInstance(HINSTANCE hInstance) override final;

	HWND GetSafeHwnd() override final;
	HWND GetParentHwnd() override final;


// IDesktopWindowCallback interface methods implementation
// This interface should be overriden in specialized class.
public:
	virtual bool OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;


protected:
	HWND							m_hWnd;
	std::wstring					m_wsClassName;
	std::wstring					m_wsWindowName;
	DWORD							m_dwStyle;
	DWORD							m_dwExStyle;
	int								m_x;
	int								m_y;
	int								m_nWidth;
	int								m_nHeight;
	HWND							m_hWndParent;
	HMENU							m_hMenu;
	HINSTANCE						m_hInstance;
};


} // namespace winapi
} // namespace asw

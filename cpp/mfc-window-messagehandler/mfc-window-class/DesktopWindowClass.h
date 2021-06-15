#pragma once

#include <string>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>
#undef	WIN32_LEAN_AND_MEAN

#include "DesktopWindowClass_intf.h"
#include "DesktopWindowCallback_intf.h"

class CDesktopWindowClass : public IDesktopWindowClass
{
public:
	class Factory
	{
	public:
		virtual std::shared_ptr<IDesktopWindowClass> CreateDesktopWindowClass() const;
	};

	CDesktopWindowClass();
	~CDesktopWindowClass();

// IDesktopWindowClass interface methods
public:
	virtual bool Register() override;
	virtual bool Unregister() override;

	/// <include file="../Debug/mfc-window.xml" path="doc/members/member[@name='M:asw.winapi.IDesktopWindowClass.SetStyle(System.UInt32)']/*" />
	virtual bool SetStyle(UINT uStyle) override;
	virtual bool SetClassExtra(int sbClsExtra) override;
	virtual bool SetInstance(HINSTANCE hInstance) override;
	virtual bool SetIcon(HICON hIcon) override;
	virtual bool SetIconSmall(HICON hIconSmall) override;
	virtual bool SetCursor(HCURSOR hCursor) override;
	virtual bool SetBackgroundBrush(HBRUSH hBackgroundBrush) override;
	virtual bool SetMenuName(const std::wstring & wsMenuName) override;
	virtual bool SetClassName(const std::wstring & wsClassName) override;

private:
	ATOM		m_windowClassAtom;
	WNDCLASSEX	m_wndClassEx;
};

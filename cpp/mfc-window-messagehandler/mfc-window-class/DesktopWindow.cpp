#include "DesktopWindow.h"
#include "DesktopWindowMessageDef.h"

#include <iostream>

std::shared_ptr<IDesktopWindow> CDesktopWindow::Factory::CreateDesktopWindow() const
{
	std::shared_ptr<CDesktopWindow> spImpl = std::make_shared<CDesktopWindow>();
	return spImpl;
}

CDesktopWindow::CDesktopWindow()
{
	m_hWnd = NULL;
	m_wsClassName = L"";
	m_wsWindowName = L"";
	m_dwStyle = NULL;
	m_dwExStyle = NULL;
	m_x = 0;
	m_y = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_hWndParent = NULL;
	m_hMenu = NULL;
	m_hInstance = ::GetModuleHandle(NULL);
}

CDesktopWindow::~CDesktopWindow()
{
	Destroy();
}

bool CDesktopWindow::Create()
{
	// Check if window is not already created
	if (m_hWnd != NULL)
	{
		// JMTODO: Error
		return false;
	}

	// Create new window
	m_hWnd = ::CreateWindowEx(
		m_dwExStyle,						// extended window style of the window being created
		m_wsClassName.c_str(),				// null-terminated class name string
		m_wsWindowName.c_str(),				// null-terminated window name string
		m_dwStyle,							// style of the window
		m_x,								// initial horizontal position of the window
		m_y,								// initial vertical position of the window
		m_nWidth,							// window width
		m_nHeight,							// window height
		m_hWndParent,						// handle of the parent or owner
		m_hMenu,							// handle to a menu
		m_hInstance,						// handle to the instance of module associated with the window
		(IDesktopWindowCallback *)(this)	// data for WM_NCCREATE message
											// (pointer to IDesktopWindowCallback interface implementation)
	);

	// Check if window creation was successful
	if (m_hWnd == NULL)
	{
		// JMTODO: Error
		return false;
	}

	return true;
}

bool CDesktopWindow::Destroy()
{
	if (m_hWnd == NULL)
	{
		// JMTODO: Error - can't destroy null window
		return false;
	}

	if (::DestroyWindow(m_hWnd) == NULL)
	{
		// JMTODO: Error
		return false;
	}

	m_hWnd = NULL;

	return true;
}

bool CDesktopWindow::SetClassName(const std::wstring & wsClassName)
{
	m_wsClassName = wsClassName;
	return true;
}

bool CDesktopWindow::SetWindowName(const std::wstring & wsWindowName)
{
	m_wsWindowName = wsWindowName;
	return true;
}

bool CDesktopWindow::CDesktopWindow::SetStyle(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
	return true;
}

bool CDesktopWindow::SetExStyle(DWORD dwExStyle)
{
	m_dwExStyle = dwExStyle;
	return true;
}

bool CDesktopWindow::SetX(int x)
{
	m_x = x;
	return true;
}

bool CDesktopWindow::SetY(int y)
{
	m_y = y;
	return true;
}

bool CDesktopWindow::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
	return true;
}

bool CDesktopWindow::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
	return true;
}

bool CDesktopWindow::SetParent(HWND hWndParent)
{
	if (::IsWindow(hWndParent) == false)
	{
		// JMTODO: Error
		return false;
	}

	m_hWndParent = hWndParent;
	return true;
}

bool CDesktopWindow::SetMenu(HMENU hMenu)
{
	m_hMenu = hMenu;
	return true;
}

bool CDesktopWindow::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	return true;
}

HWND CDesktopWindow::GetSafeHwnd()
{
	return (this == nullptr ? NULL : m_hWnd);
}

HWND CDesktopWindow::GetParentHwnd()
{
	return m_hWndParent;
}

bool CDesktopWindow::OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Implement this method in specialized class to implment your specific 
	// window message procedure.

	// Return values requirement:
	//   - true  = message has been handled and shouldn't be processed further
	//   - false = message hasn't been handled and should be processed elsewhere

	return false;
}

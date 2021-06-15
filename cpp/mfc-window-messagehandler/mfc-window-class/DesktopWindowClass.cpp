#include "DesktopWindowClass.h"
#include "DesktopWindow.h"

#include <iostream>

///	<summary>WindowProc function common for every IDesktopWindowCallback implementation.</summary>
/// <remarks>Don't use this method in any other way than through IDesktopWindowCallback interface.</remarks>
LRESULT CALLBACK InternalWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IDesktopWindowCallback * pWindowMessageCallback = nullptr;

	// Catch window pre-creation message and store IDesktopWindowCallback pointer to window data
	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT pCS = (LPCREATESTRUCT)(lParam);

		if (pCS && pCS->lpCreateParams)
		{
			// Retrieve IWindowMessageCallback pointer from lParam
			pWindowMessageCallback = static_cast<IDesktopWindowCallback *>((LPCREATESTRUCT(lParam))->lpCreateParams);

			// Save IWindowMessageCallback pointer in window data
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(pWindowMessageCallback));
		}
	}

	// Retrieve IWindowMessageCallback pointer
	pWindowMessageCallback = (IDesktopWindowCallback *)(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Catch window post-destroy message and remove IDesktopWindowCallback pointer from window data
	//  - after handling this message callback object doesn't exist anymore
	//  - still, this message will be the last one passed into IDesktopWindowCallback
	if (uMsg == WM_NCDESTROY)
	{
		// Remove IWindowMessageCallback pointer from window data
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
	}

	bool bWindowMessageHandled = false;

	// If callback found, call the window-specific message handler
	if (pWindowMessageCallback)
	{
		bWindowMessageHandled = pWindowMessageCallback->OnMessage(hWnd, uMsg, wParam, lParam);
	}

	// If window message was processed, we shouldn't call default window procedure anymore
	if (bWindowMessageHandled)
	{
		return true;
	}

	// Execute default system window procedure for unmanaged windows
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///	<summary>Maximum allowed size of string names used in WNDCLASSEX struct.</summary>
/// <remarks>This is defined in documentation for 'WNDCLASSEX structure' here:
/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
/// </remarks>
static const unsigned int DEKTOP_WINDOW_NAME_SIZE_MAX = 256;

std::shared_ptr<IDesktopWindowClass> CDesktopWindowClass::Factory::CreateDesktopWindowClass() const
{
	std::shared_ptr<IDesktopWindowClass> spImpl = std::make_shared<CDesktopWindowClass>();
	return spImpl;
}

CDesktopWindowClass::CDesktopWindowClass()
{
	m_windowClassAtom = NULL;
	m_wndClassEx = {};

	m_wndClassEx.cbSize = sizeof(m_wndClassEx);
	m_wndClassEx.style = NULL;
	m_wndClassEx.lpfnWndProc = InternalWindowProc;
	m_wndClassEx.cbClsExtra = 0;
	m_wndClassEx.cbWndExtra = 0;
	m_wndClassEx.hInstance = NULL;
	m_wndClassEx.hIcon = NULL;
	m_wndClassEx.hCursor = (::LoadCursor(NULL, IDC_ARROW));
	m_wndClassEx.hbrBackground = (HBRUSH)(::GetStockObject(NULL_BRUSH));
	m_wndClassEx.lpszMenuName = new WCHAR[DEKTOP_WINDOW_NAME_SIZE_MAX];
	m_wndClassEx.lpszClassName = new WCHAR[DEKTOP_WINDOW_NAME_SIZE_MAX];
	m_wndClassEx.hIconSm = NULL;

	ZeroMemory((void *)(m_wndClassEx.lpszMenuName), sizeof(m_wndClassEx.lpszMenuName));
	ZeroMemory((void *)(m_wndClassEx.lpszClassName), sizeof(m_wndClassEx.lpszClassName));
}

CDesktopWindowClass::~CDesktopWindowClass()
{
	Unregister();

	delete m_wndClassEx.lpszMenuName;
	delete m_wndClassEx.lpszClassName;
}

bool CDesktopWindowClass::Register()
{
	// Register new class
	m_windowClassAtom = ::RegisterClassEx(&m_wndClassEx);

	// Check if registration succeeded
	if (m_windowClassAtom == NULL)
	{
		// JMTODO: error
		return false;
	}

	return true;
}

bool CDesktopWindowClass::Unregister()
{
	if (::UnregisterClass(m_wndClassEx.lpszClassName, m_wndClassEx.hInstance) == FALSE)
	{
		// JMTODO: error
		return false;
	}	

	return true;
}

bool CDesktopWindowClass::SetStyle(UINT uStyle)
{
	m_wndClassEx.style = uStyle;
	return true;
}

bool CDesktopWindowClass::SetClassExtra(int sbClsExtra)
{
	m_wndClassEx.cbClsExtra = sbClsExtra;
	return true;
}

bool CDesktopWindowClass::SetInstance(HINSTANCE hInstance)
{
	m_wndClassEx.hInstance = hInstance;
	return true;
}

bool CDesktopWindowClass::SetIcon(HICON hIcon)
{
	m_wndClassEx.hIcon = hIcon;
	return true;
}

bool CDesktopWindowClass::SetIconSmall(HICON hIconSmall)
{
	m_wndClassEx.hIconSm = hIconSmall;
	return true;
}

bool CDesktopWindowClass::SetCursor(HCURSOR hCursor)
{
	m_wndClassEx.hCursor = hCursor;
	return true;
}

bool CDesktopWindowClass::SetBackgroundBrush(HBRUSH hBackgroundBrush)
{
	m_wndClassEx.hbrBackground = hBackgroundBrush;
	return true;
}

bool CDesktopWindowClass::SetMenuName(const std::wstring & wsMenuName)
{
	if (wsMenuName.size() > DEKTOP_WINDOW_NAME_SIZE_MAX)
	{
		// JMTODO: Error - too long
		return false;
	}

	wcscpy((WCHAR *)(m_wndClassEx.lpszMenuName), (WCHAR *)(wsMenuName.c_str()));
	return true;
}

bool CDesktopWindowClass::SetClassName(const std::wstring & wsClassName)
{
	if (wsClassName.size() > DEKTOP_WINDOW_NAME_SIZE_MAX)
	{
		// JMTODO: Error - too long
		return false;
	}

	wcscpy((WCHAR *)(m_wndClassEx.lpszClassName), (WCHAR *)(wsClassName.c_str()));
	return true;
}

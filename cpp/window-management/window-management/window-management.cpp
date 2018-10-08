#include "stdafx.h"

#include <iostream>
#include <string>
#include <memory.h>
#include <windows.h>

#define TCHAR_BUFFER_SIZE 1024

BOOL __stdcall EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR windowText[TCHAR_BUFFER_SIZE];
	INT len;

	len = ::GetWindowText(hwnd, windowText, TCHAR_BUFFER_SIZE);
	if (len > 0)
	{
		std::wcout << hwnd << L": " << std::wstring(windowText) << std::endl;
	}
	
	return true;
}

LRESULT __stdcall ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	std::cout << "hooked" << std::endl;

	return 0;
}

void WatchWindowsUICoreClass()
{
	while (true)
	{
		HWND hWnd = ::FindWindowEx(NULL, NULL, L"Windows.UI.Core.CoreWindow", NULL);

		if (hWnd)
		{
			std::cout << "\ractive" << std::endl;
		}
		else
		{
			std::cout << "\rinactive" << std::endl;
		}

		::Sleep(500);
	}
}

void ListWindowOfClass(TCHAR * windowClass)
{
	HWND hWnd = NULL;
	TCHAR windowText[TCHAR_BUFFER_SIZE];
	TCHAR windowClassName[TCHAR_BUFFER_SIZE];
	WINDOWINFO windowInfo;
	BOOL isVisible;
	BOOL isWindow;
	BOOL isZoomed;
	BOOL isIconic;

	while (hWnd = ::FindWindowEx(NULL, NULL, windowClass, NULL))
	{
		::GetWindowText(hWnd, windowText, TCHAR_BUFFER_SIZE);
		::GetWindowInfo(hWnd, &windowInfo);
		::GetClassName(hWnd, windowClassName, TCHAR_BUFFER_SIZE);
		isVisible = ::IsWindowVisible(hWnd);
		isWindow = ::IsWindow(hWnd);
		isZoomed = ::IsZoomed(hWnd);
		isIconic = ::IsIconic(hWnd);

		std::wcout << windowText << std::endl;
		std::wcout << "visible = " << isVisible << std::endl;
		std::wcout << "window = " << isWindow << std::endl;
		std::wcout << "zoomed = " << isZoomed << std::endl;
		std::wcout << "iconic = " << isIconic << std::endl;
		std::wcout << std::endl;
	}
}

int main()
{
	HWND hWnd;
	HWND hWnd_parent;
	//::EnumWindows(EnumWindowsProc, NULL);


	//HHOOK hook;
	//hook = ::SetWindowsHookEx(WH_SHELL, ShellProc, NULL, 0);
	//::UnhookWindowsHookEx(hook);

	TCHAR windowClassName[TCHAR_BUFFER_SIZE];
	TCHAR windowText[TCHAR_BUFFER_SIZE];
	WINDOWINFO windowInfo;
	WNDCLASSEX windowClassEx;
	BOOL ret;
/*
	::Sleep(3000);
	hWnd = ::FindWindowEx(NULL, NULL, NULL, L"Action center");
	ret = ::GetClassName(hWnd, windowClassName, TCHAR_BUFFER_SIZE);
	ret = ::GetWindowInfo(hWnd, &windowInfo);

	::Sleep(3000);
	ret = ::GetClassInfoEx(NULL, L"Windows.UI.Core.CoreWindow", &windowClassEx);
	DWORD e = ::GetLastError();

	::Sleep(3000);
	hWnd = ::FindWindowEx(NULL, NULL, NULL, L"Network settings");
	::GetClassName(hWnd, windowClassName, TCHAR_BUFFER_SIZE);


	::Sleep(3000);
	hWnd = ::FindWindowEx(NULL, NULL, L"Windows.UI.Core.CoreWindow", NULL);
	::GetWindowText(hWnd, windowText, TCHAR_BUFFER_SIZE);
*/
	
	//WatchWindowsUICoreClass();

	::Sleep(3000);
	ListWindowOfClass(L"Windows.UI.Core.CoreWindow");

    return 0;
}

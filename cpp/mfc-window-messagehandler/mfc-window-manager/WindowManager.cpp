#include "WindowManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

LRESULT CALLBACK InternalWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CDesktopWindowManager::CreateDesktopWindowClass(SDesktopWindowClassDesc & sWindowClassDesc)
{
	WNDCLASSEX wndClassEx = {};
	wndClassEx.cbSize = sizeof(wndClassEx);
	wndClassEx.style = NULL;
	wndClassEx.lpfnWndProc = InternalWindowProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = NULL;
	wndClassEx.hIcon = NULL;
	wndClassEx.hCursor = NULL;
	wndClassEx.hbrBackground = NULL;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.lpszClassName = sWindowClassDesc.windowClassName.c_str();
	wndClassEx.hIconSm = NULL;

	ATOM windowClassAtom = ::RegisterClassEx(&wndClassEx);

	if (windowClassAtom == NULL)
	{
		// JMTODO: Error
		return;
	}
}

void CDesktopWindowManager::CreateDesktopWindow(SDesktopWindowDesc & sWindowDesc)
{
	SDesktopWindowExtras sDesktopWindowExtras;

	HWND hWnd = ::CreateWindowEx(
		NULL,
		sWindowDesc.windowClassName.c_str(),
		sWindowDesc.windowName.c_str(),
		NULL,
		0,
		0,
		0,
		0,
		NULL,
		NULL,
		::GetModuleHandle(NULL),
		(LPVOID)(&sDesktopWindowExtras)
	);

	if (hWnd == NULL)
	{
		// JMTODO: Error
		return;
	}
}

LRESULT CALLBACK InternalWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
#pragma once

#include <windows.h>

class Window2
{
public:
	bool Create();
	bool Destroy();

private:
	bool RegisterWindowClass();
	bool UnregisterWindowClass();

	/*

			UNFINISHED

	*/

	static LRESULT CALLBACK internalWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

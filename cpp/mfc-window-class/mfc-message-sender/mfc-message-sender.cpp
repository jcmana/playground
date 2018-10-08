#include <string>
#include <windows.h>

int main()
{
	HWND hWindow;

	std::wstring wsWindowClassName = L"mfc-window-class";
	std::wstring wsWindowName = L"";

	hWindow = FindWindow(
		/* lpClassName */	wsWindowClassName.c_str(),
		/* lpWindowName */	NULL
	);

	SendMessage(
		/* hWnd */			hWindow,
		/* Msg */			WM_COPYDATA,
		/* wParam */		NULL,
		/* lParam */		NULL
	);


    return 0;
}


#include "Application.h"

BOOL CApplication::InitInstance()
{
	CWnd * window = new CWnd();

	window->CreateEx(
		/* extented style */	NULL,
		/* class name */		NULL,
		/* window name */		L"MFC Classes - CWnd",
		/* style */				WS_CHILD,
		/* */					RECT {100, 100, 0, 0},
		/* parent window */		nullptr,
		/* menu */				0);

	m_pMainWnd = window;

	//
	// DOESN'T WORK - ::CreateWindowEx() fails.
	//
	
	window->ShowWindow(SW_NORMAL);
	window->UpdateWindow();

	return TRUE;
}
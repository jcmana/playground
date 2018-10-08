#pragma once

#include <sdkddkver.h>
#include <afxwin.h>
#include <iostream>

/*
	Description:
		Main Application Window message handling method.

	Parameters:
		hwnd	- A handle to the window. 
		uMsg	- The message.
		wParam	- Additional message information. The contents of this parameter depend on the value of the uMsg parameter. 
		lParam	- Additional message information. The contents of this parameter depend on the value of the uMsg parameter. 

	Return value:
		Result of the message processing.
*/
LRESULT ApplicationWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

#include <string>
#include <windows.h>

static HDC hDc = NULL;
static BOOL fDraw = FALSE;
static POINT ptPrevious;

LRESULT WindowMessageHandler(
	HWND   hWnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			fDraw = TRUE;
			ptPrevious.x = LOWORD(lParam);
			ptPrevious.y = HIWORD(lParam);
			return 0L;
		}
		break;

		case WM_LBUTTONUP:
		{
			if (fDraw)
			{
				hDc = ::GetDC(hWnd);
				{
					::MoveToEx(hDc, ptPrevious.x, ptPrevious.y, NULL);
					::LineTo(hDc, LOWORD(lParam), HIWORD(lParam));
				}
				::ReleaseDC(hWnd, hDc);
			}
			fDraw = FALSE;
			return 0L;
		}
		break;

		case WM_MOUSEMOVE:
		{
			if (fDraw)
			{
				hDc = ::GetDC(hWnd);
				{
					::MoveToEx(hDc, ptPrevious.x, ptPrevious.y, NULL);
					::LineTo(hDc, ptPrevious.x = LOWORD(lParam), ptPrevious.y = HIWORD(lParam));
				}
				::ReleaseDC(hWnd, hDc);
			}
			return 0L;
		}
		break;

		case WM_CLOSE:
		{
			::DestroyWindow(hWnd);
			return 0L;
		}
		break;

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0L;
		}
		break;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	std::string className = "windowDirectDrawingClass";
	std::string windowName = "windowDirectDrawing";

	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(sWindowClass);
	sWindowClass.style = 0;
	sWindowClass.lpfnWndProc = (WNDPROC)(WindowMessageHandler);
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;
	sWindowClass.hInstance = hInstance;
	sWindowClass.hIcon = NULL;
	sWindowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW); ;
	sWindowClass.hbrBackground = ::CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = className.c_str();
	sWindowClass.hIconSm = NULL;

	ATOM aWindowClass = ::RegisterClassEx(&sWindowClass);

	if (aWindowClass == NULL)
	{
		return 0;
	}

	HWND hWindow = ::CreateWindowEx(
		NULL,							// dwStyleEx 
		className.c_str(),				// lpClassName 
		windowName.c_str(),				// lpWindowName  
		WS_OVERLAPPEDWINDOW,			// dwStyle 
		100,							// x 
		100,							// y 
		800,							// nWidth 
		600,							// nHeight 
		NULL,							// hWndParent 
		NULL,							// hMenu 
		sWindowClass.hInstance,			// hInstance 
		NULL							// lpParam 
	);

	if (hWindow == NULL)
	{
		return 0;
	}

	::ShowWindow(hWindow, nCmdShow);

	MSG sMessage;
	BOOL bResult;

	while ((bResult = ::GetMessage(&sMessage, NULL, 0, 0)) != NULL)
	{
		// handle quit code
		if (bResult == 0)
		{
			break;
		}

		// handle error code
		if (bResult == -1)
		{
			continue;
		}

		// handle the window message
		::TranslateMessage(&sMessage);
		::DispatchMessage(&sMessage);
	}

	return 0;
}

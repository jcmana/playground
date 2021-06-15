#include "Application.h"

CApplication::CApplication()
{
	m_wsWindowClassName = L"mfc-window-class";
	m_wsWindowName = L"MFC Window class";
}

BOOL CApplication::InitInstance()
{
	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(sWindowClass);
	sWindowClass.style = 0;
	sWindowClass.lpfnWndProc = (WNDPROC)(ApplicationWindowProc);
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;
	sWindowClass.hInstance = AfxGetInstanceHandle();
	sWindowClass.hIcon = NULL;
	sWindowClass.hCursor = NULL;
	sWindowClass.hbrBackground = NULL;
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = m_wsWindowClassName.c_str();
	sWindowClass.hIconSm = NULL;

	ATOM aWindowClass = RegisterClassEx(&sWindowClass);


	HWND hWindow = CreateWindowEx(
		/* dwStyleEx */		NULL, //WS_EX_TRANSPARENT,
		/* lpClassName */	m_wsWindowClassName.c_str(),
		/* lpWindowName  */	m_wsWindowName.c_str(),
		/* dwStyle */		WS_VISIBLE,
		/* x */				100,
		/* y */				100,
		/* nWidth */		800,
		/* nHeight */		600,
		/* hWndParent */	HWND_MESSAGE,
		/* hMenu */			NULL,
		/* hInstance */		sWindowClass.hInstance,
		/* lpParam */		NULL
	);


	m_notificationArea.Initialize(hWindow, -3);
	m_notificationArea.CreateIcon();
	m_notificationArea.SetTooltip(L"jebu vam auticka");
	m_notificationArea.HideIcon();
	m_notificationArea.ShowIcon();
//	m_notificationArea.DeleteIcon();

	m_notificationArea2.Initialize(hWindow, 777);
	m_notificationArea2.CreateIcon();
	m_notificationArea2.SetTooltip(L"jebu vam uplne vsecko");
	
	// Timer is recurring; event is triggered until KillTimer() call
	//UINT timerID = ::SetTimer(hWindow, NULL, 5000, NULL);


	// This is bullshit to have message loop inside InitInstance() method. But I don't 
	// have CWnd object to assign to m_mainWnd member to get MFC do it for me.

	BOOL bRet;
	MSG mMessage;

	while (1)
	{
		bRet = ::GetMessage(&mMessage, NULL, 0, 0);

		// Message received
		if (bRet > 0)
		{
			::TranslateMessage(&mMessage);

			::DispatchMessage(&mMessage);

			continue;
		}
		
		// Message error
		if (bRet < 0)
		{
			continue;
		}

		// Message QUIT
		if (bRet == 0)
		{
			break;
		}
	}

	return TRUE;
}

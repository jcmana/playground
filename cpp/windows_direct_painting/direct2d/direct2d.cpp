// direct2d.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <d2d1.h>
#include <d2d1_1.h>

#include "direct2d.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ID2D1Factory *			d2FactoryPtr;
ID2D1HwndRenderTarget *	d2RenderTargetPtr;
ID2D1SolidColorBrush *	d2LightSlateGrayBrushPtr;
ID2D1SolidColorBrush *	d2CornflowerBlueBrushPtr;
ID2D1SolidColorBrush *	d2BlackBrushPtr;

FLOAT d2SceneDX = 0.0f;
FLOAT d2SceneDY = 0.0f;
FLOAT d2SceneSX = 1.0f;
FLOAT d2SceneSY = 1.0f;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				Render();

template<class Interface>
inline void SafeRelease(Interface ** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (InitInstance (hInstance, nCmdShow) == FALSE)
    {
        return FALSE;
    }

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2D));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	CoUninitialize();

	SafeRelease(&d2FactoryPtr);
	SafeRelease(&d2RenderTargetPtr);
	SafeRelease(&d2LightSlateGrayBrushPtr);
	SafeRelease(&d2CornflowerBlueBrushPtr);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	HRESULT hRes;
	hRes = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2FactoryPtr);
	if (FAILED(hRes)) return FALSE;

	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// Create a Direct2D render target
	hRes = d2FactoryPtr->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size),
		&d2RenderTargetPtr
	);
	if (FAILED(hRes)) return FALSE;

	// Create a gray brush
	hRes = d2RenderTargetPtr->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		&d2LightSlateGrayBrushPtr
	);
	if (FAILED(hRes)) return FALSE;

	// Create a blue brush
	hRes = d2RenderTargetPtr->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
		&d2CornflowerBlueBrushPtr
	);
	if (FAILED(hRes)) return FALSE;

	// Create a black brush
	hRes = d2RenderTargetPtr->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&d2BlackBrushPtr
	);
	if (FAILED(hRes)) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
				case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
				default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT: d2SceneDX -= 10.0f / d2SceneSX; break;
				case VK_RIGHT: d2SceneDX += 10.0f / d2SceneSX; break;
				case VK_UP: d2SceneDY -= 10.0f / d2SceneSY; break;
				case VK_DOWN: d2SceneDY += 10.0f / d2SceneSY; break;
				case VK_ADD: d2SceneSX += 0.5f; d2SceneSY += 0.5f; break;
				case VK_SUBTRACT: d2SceneSX -= 0.5f; d2SceneSY -= 0.5f; break;
			}

			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

		case WM_PAINT:
		{
			Render();
			ValidateRect(hWnd, NULL);
		}
		break;

		case WM_SIZE:
		{
			if (d2RenderTargetPtr)
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				d2RenderTargetPtr->Resize(D2D1::SizeU(width, height));
			}
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Render()
{
	d2RenderTargetPtr->BeginDraw();

	D2D1_SIZE_F rtSize = d2RenderTargetPtr->GetSize();

	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	D2D1::Matrix3x2F d2SceneTranslate = D2D1::Matrix3x2F::Translation(d2SceneDX, d2SceneDY);
	D2D1::Matrix3x2F d2SceneScale = D2D1::Matrix3x2F::Scale(d2SceneSX, d2SceneSY, D2D1::Point2F(width / 2, height / 2));

	d2RenderTargetPtr->SetTransform(d2SceneTranslate * d2SceneScale);
	d2RenderTargetPtr->Clear(D2D1::ColorF(D2D1::ColorF::White));
	//d2RenderTargetPtr->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);


	ID2D1PathGeometry * d2PathGeometryPtr = nullptr;
	d2FactoryPtr->CreatePathGeometry(&d2PathGeometryPtr);

/*
	for (int x = 0; x < width; x += 10)
	{
		d2RenderTargetPtr->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
			d2BlackBrushPtr,
			1.0f
		);
	}

	for (int y = 0; y < height; y += 10)
	{
		d2RenderTargetPtr->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
			d2BlackBrushPtr,
			1.0f);
	}
*/

	// Draw two rectangles.
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - 50.0f,
		rtSize.height / 2 - 50.0f,
		rtSize.width / 2 + 50.0f,
		rtSize.height / 2 + 50.0f
	);

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		rtSize.width / 2 - 100.0f,
		rtSize.height / 2 - 100.0f,
		rtSize.width / 2 + 100.0f,
		rtSize.height / 2 + 100.0f
	);

	// Draw a filled rectangle.
	d2RenderTargetPtr->FillRectangle(&rectangle1, d2LightSlateGrayBrushPtr);

	// Draw the outline of a rectangle.
	d2RenderTargetPtr->DrawRectangle(&rectangle2, d2CornflowerBlueBrushPtr);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(rtSize.width / 2, rtSize.height / 2), 30.0f, 30.0f);

	d2RenderTargetPtr->DrawEllipse(&ellipse, d2BlackBrushPtr);

	ID2D1GeometrySink * d2SinkPtr = nullptr;
	d2PathGeometryPtr->Open(&d2SinkPtr);
	d2SinkPtr->BeginFigure(D2D1::Point2F(20.0f, 20.0f), D2D1_FIGURE_BEGIN_FILLED);
	d2SinkPtr->AddLine(D2D1::Point2F(200.0f, 20.0f));
	d2SinkPtr->AddLine(D2D1::Point2F(210.0f, 100.0f));
	d2SinkPtr->AddLine(D2D1::Point2F(10.0f, 100.0f));
	d2SinkPtr->EndFigure(D2D1_FIGURE_END_CLOSED);
	d2SinkPtr->Close();
	d2SinkPtr->Release();

	d2RenderTargetPtr->DrawGeometry(d2PathGeometryPtr, d2BlackBrushPtr);
	d2RenderTargetPtr->FillGeometry(d2PathGeometryPtr, d2CornflowerBlueBrushPtr);

	d2PathGeometryPtr->Release();

	d2RenderTargetPtr->EndDraw();
}

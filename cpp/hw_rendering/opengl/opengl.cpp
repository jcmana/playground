#include <iostream>
#include <string>
#include <exception>
#include <windows.h>
#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/GL.h>

HDC windowDeviceContext = NULL;
HGLRC windowContext = NULL;
GLuint vertexBufferID = 0;

void InitializeRendering(HWND hWnd)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	windowDeviceContext = GetDC(hWnd);

	int pixelFormat;
	pixelFormat = ChoosePixelFormat(windowDeviceContext, &pfd);
	SetPixelFormat(windowDeviceContext, pixelFormat, &pfd);

	windowContext = wglCreateContext(windowDeviceContext);
	wglMakeCurrent(windowDeviceContext, windowContext);

	if (glewInit() != GLEW_OK)
	{
		throw std::exception("Failed to initialized GLEW.");
	}
}

void DeinitializeRendering()
{
	wglDeleteContext(windowContext);
}

void InitializeGeometry()
{
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertexBufferData[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	// Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized
		0,                  // stride
		nullptr             // array buffer offset
	);
}

void DeinitializeGeometry()
{
	glDeleteBuffers(1, &vertexBufferID);
}

void WindowRender()
{
	glDrawBuffer(GL_FRONT_AND_BACK);

	// Clear the window
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	// Swap main plane buffers
	SwapBuffers(windowDeviceContext);
}

LRESULT WindowMessageHandler(
	HWND   hWnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			InitializeRendering(hWnd);
			InitializeGeometry();
		}
		break;

		case WM_SIZE:
		{
			InitializeGeometry();
		}
		break;

		case WM_PAINT:
		{
			WindowRender();
		}
		break;

		case WM_ERASEBKGND:
		{
			return TRUE;
		}
		break;

		case WM_CLOSE:
		{
			DeinitializeGeometry();
			DeinitializeRendering();
			DestroyWindow(hWnd);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		default:
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	}

	return 0L;
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	std::string className = "openglWindowClass";
	std::string windowName = "openglWindow";

	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(sWindowClass);
	sWindowClass.style = CS_OWNDC;
	sWindowClass.lpfnWndProc = (WNDPROC)(WindowMessageHandler);
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;
	sWindowClass.hInstance = hInstance;
	sWindowClass.hIcon = NULL;
	sWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW); ;
	sWindowClass.hbrBackground = CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = className.c_str();
	sWindowClass.hIconSm = NULL;

	ATOM aWindowClass = RegisterClassEx(&sWindowClass);

	if (aWindowClass == NULL)
	{
		return 0;
	}

	HWND hWindow = CreateWindowEx(
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

	ShowWindow(hWindow, nCmdShow);

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
		TranslateMessage(&sMessage);
		DispatchMessage(&sMessage);
	}

	return 0;
}

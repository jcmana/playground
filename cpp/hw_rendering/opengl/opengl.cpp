#include <iostream>
#include <fstream>
#include <streambuf>
#include <memory>
#include <string>
#include <exception>

#include <windows.h>
#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "Shader.h"
#include "Program.h"

HDC windowDeviceContext = NULL;
HGLRC windowContext = NULL;
GLuint vertexBufferID = 0;

std::unique_ptr<Shader> upVertexShader;
std::unique_ptr<Shader> upFragmentShader;
std::unique_ptr<Program> upProgram;

void InitializeRendering(HWND hWnd)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE,    //Flags
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
		0,
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
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(windowContext);
}

void InitializeGeometry()
{
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertexBufferData[] =
	{
		-0.8f, -0.8f, 0.0f,		+0.8f, -0.8f, 0.0f,
		+0.8f, -0.8f, 0.0f,		+0.2f, +0.8f, 0.0f,
		+0.2f, +0.8f, 0.0f,		-0.6f, +0.6f, 0.0f,
		-0.6f, +0.6f, 0.0f,		-0.8f, -0.8f, 0.0f,
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

void InitializeShaders()
{
	{
		auto shaderFile = std::ifstream("C:/dev/git/playground/cpp/hw_rendering/opengl/vertexShader.glsl");
		auto shaderCode = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
		upVertexShader = std::make_unique<Shader>(GL_VERTEX_SHADER, shaderCode);
	}

	{
		auto shaderFile = std::ifstream("C:/dev/git/playground/cpp/hw_rendering/opengl/fragmentShader.glsl");
		auto shaderCode = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
		upFragmentShader = std::make_unique<Shader>(GL_FRAGMENT_SHADER, shaderCode);
	}

	upProgram = std::make_unique<Program>();
	upProgram->Attach(*upVertexShader);
	upProgram->Attach(*upFragmentShader);
	upProgram->Link();
}

void DeinitializeGeometry()
{
	glDeleteBuffers(1, &vertexBufferID);
}

void WindowRender()
{
	glDrawBuffer(GL_BACK);

	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Draw the vertex buffer
	glUseProgram(upProgram->Id());
	glDrawArrays(GL_LINES, 0, 8);

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
			InitializeShaders();
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

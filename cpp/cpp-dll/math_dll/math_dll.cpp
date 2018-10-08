// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  

#include "math_dll.h"

namespace MathLibrary
{

double Functions::Add(double a, double b)
{
	return a + b;
}

double Functions::Multiply(double a, double b)
{
	return a * b;
}

double Functions::AddMultiply(double a, double b)
{
	return a + (a * b);
}

void Functions::IncrementConstant()
{
	int * ref = &counter;
	++counter;
}


}

#include <windows.h>

BOOL WINAPI DllMain(
	IN HINSTANCE hinstDLL,
	IN DWORD     fdwReason,
	IN LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{

		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		default:
			break;
	}

	return TRUE;
}

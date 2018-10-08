#include "ShellWrapper.h"

// Shell32.dll function signatures
using SIG_SHGetPropertyStoreForWindow = HRESULT (__stdcall *)(HWND, REFIID, void **);

CShellWrapper::CShellWrapper()
{
	// Load Shell32.dll library into memory
	m_hShellModule = LoadLibrary(L"Shell32.dll");
}

CShellWrapper::~CShellWrapper()
{
	// Free Shell32.dll library
	FreeLibrary(m_hShellModule);
}

HRESULT CShellWrapper::SHGetPropertyStoreForWindow(
	_In_  HWND   hwnd,
	_In_  REFIID riid,
	_Out_ void   **ppv
)
{
	SIG_SHGetPropertyStoreForWindow pSHGetPropertyStoreForWindow = (SIG_SHGetPropertyStoreForWindow)(GetProcAddress(m_hShellModule, "SHGetPropertyStoreForWindow"));
	return pSHGetPropertyStoreForWindow(hwnd, riid, ppv);
}
#pragma once

#include <ShlObj.h>

class CShellWrapper
{
	public:
		CShellWrapper();
		~CShellWrapper();

		HRESULT SHGetPropertyStoreForWindow(
			_In_  HWND   hwnd,
			_In_  REFIID riid,
			_Out_ void   **ppv
		);

	private:
		HMODULE m_hShellModule;
};

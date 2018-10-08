#pragma once

#include <afxwin.h>
#include <objbase.h>
#include <propkey.h>
#include <propvarutil.h>
#include <wrl/client.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "Logger.h"
#include "Frame.h"
#include "ShellWrapper.h"
#include "Utils.h"

class CApplication : 
	public CWinApp,
	private CShellWrapper
{
	using PSHPROPERTYSTORE = long(__stdcall *)(HWND, REFIID, void**);	// SHGetPropertyStoreForWindow function signature

	public:
		void SetWindowIcon(HICON hIcon);
		void SetTaskbarIcon(HICON hIcon);
		void SetRelaunchIcon(HICON hIcon);

	private:
		BOOL InitInstance();

	private:
		std::wstring	m_commandLine;

		HICON			m_hIconAVAST;
		HICON			m_hIconAVG;
		HICON			m_hIconPAP;
};

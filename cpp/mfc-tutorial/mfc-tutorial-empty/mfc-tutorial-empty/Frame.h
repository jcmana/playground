#pragma once

#include <afxwin.h>
#include <propkey.h>
#include <propvarutil.h>
#include <propsys.h>

#include <string>
#include <memory>

#include "resource.h"
#include "ShellWrapper.h"

class CFrame :
	public CFrameWnd,
	private CShellWrapper
{
	public:
		CFrame();
		~CFrame();

		bool SetProperty_AppUserModel_ID(std::wstring ID);
		bool SetProperty_AppUserModel_RelaunchIconResource(std::wstring relaunchIconResource);

	private:
		IPropertyStore * m_spPropertyStore;
};

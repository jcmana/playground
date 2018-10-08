#include "Frame.h"

CFrame::CFrame()
{
	// Create CFrameWnd
	Create(NULL, _T("MFC Application Tutorial"), WS_OVERLAPPEDWINDOW, CRect(90, 120, 550, 480));

	// Initialize Window-specific IPropertyStore
	SHGetPropertyStoreForWindow(m_hWnd, IID_PPV_ARGS(&m_spPropertyStore));
}

CFrame::~CFrame()
{
	// Release Window-specific IPropertyStore
	m_spPropertyStore->Release();
}

bool CFrame::SetProperty_AppUserModel_ID(std::wstring appUserModelID)
{
	PROPVARIANT prop;
	InitPropVariantFromString(appUserModelID.c_str(), &prop);
	HRESULT hr = m_spPropertyStore->SetValue(PKEY_AppUserModel_ID, prop);

	return (hr == S_OK);
}

bool CFrame::SetProperty_AppUserModel_RelaunchIconResource(std::wstring relaunchIconResource)
{
	PROPVARIANT prop;
	InitPropVariantFromString(relaunchIconResource.c_str(), &prop);
	HRESULT hr = m_spPropertyStore->SetValue(PKEY_AppUserModel_RelaunchIconResource, prop);

	return (hr == S_OK);
}

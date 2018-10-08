#include "Utils.h"

BOOL CreateShortcut(const WCHAR * shortcutPath, const WCHAR * appUserModelID, const WCHAR * iconFilePath, int iconIndex)
{
	// get current executable path
	WCHAR exePath[MAX_PATH];
	DWORD charWritten = ::GetModuleFileName(NULL, exePath, MAX_PATH);
	HRESULT hr = charWritten > 0 ? S_OK : E_FAIL;
	if (FAILED(hr)) return false;

	hr = CoInitialize(NULL);

	// create ISHellLink object
	Microsoft::WRL::ComPtr<IShellLink> shellLink;
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));
	if (FAILED(hr)) return false;

	// set path to linked executable
	hr = shellLink->SetPath(exePath);
	if (FAILED(hr)) return false;

	hr = shellLink->SetArguments(L"");
	if (FAILED(hr)) return false;

	// get IShellLink object property store
	Microsoft::WRL::ComPtr<IPropertyStore> propertyStore;
	hr = shellLink.As(&propertyStore);
	if (FAILED(hr)) return false;

	// set IShellLink object PKEY_AppUserModel_ID property
	PROPVARIANT appIdPropVar;
	hr = InitPropVariantFromString(appUserModelID, &appIdPropVar);
	if (FAILED(hr)) return false;
	hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
	if (FAILED(hr)) return false;
	hr = propertyStore->Commit();
	if (FAILED(hr)) return false;

	// create IPersistFile object (for shortcut file)
	Microsoft::WRL::ComPtr<IPersistFile> persistFile;
	hr = shellLink.As(&persistFile);
	if (FAILED(hr)) return false;

	// resolve shortcut icon path
	WCHAR * imagePath = _wfullpath(0, iconFilePath, MAX_PATH);
	hr = imagePath != 0 ? S_OK : HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	if (FAILED(hr)) return false;

	// set shortcut icon
	hr = shellLink->SetIconLocation(imagePath, iconIndex);
	if (FAILED(hr)) return false;

	// create shortcut file
	hr = persistFile->Save(shortcutPath, TRUE);
	if (FAILED(hr)) return false;

	hr = ::PropVariantClear(&appIdPropVar);
	if (FAILED(hr)) return false;

	CoUninitialize();

	return true;
}
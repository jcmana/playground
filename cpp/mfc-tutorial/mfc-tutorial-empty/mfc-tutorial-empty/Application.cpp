#include "Application.h"

CLogger g_Log("application.log.txt");

const TCHAR * PARAM_AV	= L"avg.av";
const TCHAR * PARAM_PAP	= L"avg.pap";

BOOL CApplication::InitInstance()
{
	// Get command line parameters
	m_commandLine = GetCommandLine();

	std::wstring exectuablePath, executableParam;
	std::wstringstream(m_commandLine) >> exectuablePath >> executableParam;

	// Initialize application main window
	CFrame * pMainFrame = new CFrame();
	m_pMainWnd = pMainFrame;

	// Get current executable path
	WCHAR moduleFileName[MAX_PATH];
	DWORD nResult = GetModuleFileName(NULL, moduleFileName, MAX_PATH);

	// Set main window Application User Model ID
	pMainFrame->SetProperty_AppUserModel_ID(executableParam);

	// Create desktop shortcut
	std::wstring shortcutPath = std::wstring(L"C:\\Users\\manasek\\Desktop\\My Fucking App.lnk");
	std::wstring appUserModelID = std::wstring(L"my.fucking.app");
	std::wstring iconFilePath = std::wstring(moduleFileName);
	int iconIndex = 0 - IDI_ICON3; // we need negative value, to find icon by its reource ID
	//BOOL r = CreateShortcut(shortcutPath.c_str(), appUserModelID.c_str(), iconFilePath.c_str(), iconIndex);

	// Load icons from application resources
	m_hIconAVAST = LoadIcon(IDI_ICON1);		// Avast icon
	m_hIconAVG = LoadIcon(IDI_ICON2);		// AVG icon
	m_hIconPAP = LoadIcon(IDI_ICON3);		// PAP icon

	// Set icons
	if (executableParam == PARAM_AV)
	{
		SetWindowIcon(m_hIconAVG);
		SetTaskbarIcon(m_hIconAVG);
		pMainFrame->SetProperty_AppUserModel_RelaunchIconResource(moduleFileName + std::wstring(L",-") + std::to_wstring(IDI_ICON2));
	}
	else if (executableParam == PARAM_PAP)
	{
		SetWindowIcon(m_hIconPAP);
		SetTaskbarIcon(m_hIconPAP);
		pMainFrame->SetProperty_AppUserModel_RelaunchIconResource(moduleFileName + std::wstring(L",-") + std::to_wstring(IDI_ICON3));
	}
	else
	{
		SetWindowIcon(m_hIconAVAST);
		SetTaskbarIcon(m_hIconAVAST);
		pMainFrame->SetProperty_AppUserModel_RelaunchIconResource(moduleFileName + std::wstring(L",-") + std::to_wstring(IDI_ICON1));
	}

	//Microsoft::WRL::ComPtr<ITaskbarList3> pTaskbarList;
	//CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pTaskbarList));
	//pTaskbarList->SetOverlayIcon(m_pMainWnd->m_hWnd, m_hIconPAP, L"Popici taskbar mrda!");

	// Show main window frame
	pMainFrame->ShowWindow(SW_NORMAL);
	pMainFrame->UpdateWindow();

	return TRUE;
}

void CApplication::SetWindowIcon(HICON hIcon)
{
	// A. Using CWnd method inherited in CFrame
	GetMainWnd()->SetIcon(hIcon, FALSE);

	// B. Using CWnd method from CWnd class instantiated using HWND
	//CWnd::FromHandle(m_pMainWnd->m_hWnd)->SetIcon(hIcon, FALSE);

	// C. Using Windows message WM_SETICON
	//::SendMessage(m_pMainWnd->m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)(hIcon));
}

void CApplication::SetTaskbarIcon(HICON hIcon)
{
	// A. Using CWnd method inherited in CFrame
	//GetMainWnd()->SetIcon(hIcon, TRUE);

	// B. Using CWnd method from CWnd class instantiated using HWND
	CWnd::FromHandle(m_pMainWnd->m_hWnd)->SetIcon(hIcon, TRUE);

	// C. Using Windows message WM_SETICON
	//::SendMessage(m_pMainWnd->m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)(hIcon));
}

void CApplication::SetRelaunchIcon(HICON hIcon)
{
	HRESULT hr = NULL;

	IPropertyStore * pStore;
	hr = SHGetPropertyStoreForWindow(m_pMainWnd->m_hWnd, IID_PPV_ARGS(&pStore));

	PROPVARIANT prop1;
	InitPropVariantFromString(L"bohakristamfcappka", &prop1);
	hr = pStore->SetValue(PKEY_AppUserModel_ID, prop1);

	DWORD nSize = 255;
	WCHAR lpBuffer[255];
	::GetCurrentDirectory(nSize, lpBuffer);
	std::wstring p;
	p = lpBuffer + std::wstring(L"\\AVGUI.exe,-") + std::to_wstring(128);


	PROPVARIANT prop2;
	//InitPropVariantFromString(L"C:\\dev\\source\\playground\\cpp\\mfc-tutorial\\mfc-tutorial-empty\\mfc-tutorial-empty\\Resources\\avast.ico", &prop2);
	//InitPropVariantFromString(L"C:\\dev\\source\\playground\\cpp\\mfc-tutorial\\mfc-tutorial-empty\\Debug\\mfc-tutorial-empty.exe,-104", &prop2);
	InitPropVariantFromString(L"C:\\Program Files (x86)\\AVG\\Antivirus\\AVGUI.exe,-128", &prop2);
	//InitPropVariantFromString(L"C:\\omg\\wtf (x86)bbq\\AVGUI.exe,-128", &prop2);
	//InitPropVariantFromString(L".\\mfc-tutorial-empty.exe,-101", &prop2);
	//InitPropVariantFromString(p.c_str(), &prop2);
	hr = pStore->SetValue(PKEY_AppUserModel_RelaunchIconResource, prop2);

	//release
	hr = pStore->Commit();
	pStore->Release();
}

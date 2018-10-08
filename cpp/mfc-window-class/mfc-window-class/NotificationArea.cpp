#include "NotificationArea.h"

CNotificationArea::CNotificationArea()
{
}

void CNotificationArea::Initialize(HWND windowHandle, UINT uID)
{
	m_windowHandle = windowHandle;
	m_uID = uID;
	m_tooltipText = L"mfc-window-class tooltip";
	m_iconHandle = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
}

void CNotificationArea::Release()
{
	DestroyIcon(m_iconHandle);
}

void CNotificationArea::CreateIcon()
{
	NOTIFYICONDATA notifyIconData = {};
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = m_windowHandle;
	notifyIconData.uID = m_uID;
	notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP;
	notifyIconData.hIcon = m_iconHandle;
	memset((void *)(notifyIconData.szTip), 0, 128);
	memcpy((void *)(notifyIconData.szTip), (void *)(m_tooltipText.c_str()), (m_tooltipText.size() * sizeof(WCHAR)));
	notifyIconData.uVersion = 0;

	DWORD error;

	if (Shell_NotifyIcon(NIM_ADD, &notifyIconData) == FALSE)
	{
		error = GetLastError();

		std::cout << "GetLastError(): " << error << std::endl;
	}

	if (Shell_NotifyIcon(NIM_SETVERSION, &notifyIconData) == FALSE)
	{
		error = GetLastError();

		std::cout << "GetLastError(): " << error << std::endl;
	}
}

void CNotificationArea::DeleteIcon()
{
	NOTIFYICONDATA notifyIconData = {};
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = m_windowHandle;
	notifyIconData.uID = m_uID;

	Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}

void CNotificationArea::ShowIcon()
{
	NOTIFYICONDATA notifyIconData = {};
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = m_windowHandle;
	notifyIconData.uID = m_uID;
	notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_STATE | NIF_SHOWTIP;
	notifyIconData.hIcon = m_iconHandle;
	memset((void *)(notifyIconData.szTip), 0, 128);
	memcpy((void *)(notifyIconData.szTip), (void *)(m_tooltipText.c_str()), (m_tooltipText.size() * sizeof(WCHAR)));
	notifyIconData.dwStateMask = NIS_HIDDEN;
	notifyIconData.uVersion = NOTIFYICON_VERSION_4;

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}

void CNotificationArea::HideIcon()
{
	NOTIFYICONDATA notifyIconData = {};
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = m_windowHandle;
	notifyIconData.uID = m_uID;
	notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_STATE | NIF_SHOWTIP;
	notifyIconData.hIcon = m_iconHandle;
	memset((void *)(notifyIconData.szTip), 0, 128);
	memcpy((void *)(notifyIconData.szTip), (void *)(m_tooltipText.c_str()), (m_tooltipText.size() * sizeof(WCHAR)));
	notifyIconData.dwState = NIS_HIDDEN;
	notifyIconData.dwStateMask = NIS_HIDDEN;
	notifyIconData.uVersion = NOTIFYICON_VERSION_4;

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}

void CNotificationArea::SetTooltip(std::wstring tooltip)
{
	m_tooltipText = tooltip;

	NOTIFYICONDATA notifyIconData = {};
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = m_windowHandle;
	notifyIconData.uID = m_uID;
	notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP;
	notifyIconData.hIcon = m_iconHandle;
	memset((void *)(notifyIconData.szTip), 0, 128);
	memcpy((void *)(notifyIconData.szTip), (void *)(m_tooltipText.c_str()), (m_tooltipText.size() * sizeof(WCHAR)));
	notifyIconData.uVersion = NOTIFYICON_VERSION_4;

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}

#include "stdafx.h"

#include <iostream>
#include <string>

#include <windows.h>
#include <shellapi.h>

enum class EWatch
{
	USER_NOTIFICATION_STATE,
	USER_QUIET_HOURS
};

using PSHQueryUserNotificationState = HRESULT(__stdcall *)(QUERY_USER_NOTIFICATION_STATE *);

std::string ToString(QUERY_USER_NOTIFICATION_STATE quns)
{
	std::string str;

	switch (quns)
	{
		case QUNS_NOT_PRESENT:				str = "QUNS_NOT_PRESENT"; break;
		case QUNS_BUSY:						str = "QUNS_BUSY"; break;
		case QUNS_RUNNING_D3D_FULL_SCREEN:	str = "QUNS_RUNNING_D3D_FULL_SCREEN"; break;
		case QUNS_PRESENTATION_MODE:		str = "QUNS_PRESENTATION_MODE"; break;
		case QUNS_ACCEPTS_NOTIFICATIONS:	str = "QUNS_ACCEPTS_NOTIFICATIONS"; break;
		case QUNS_QUIET_TIME:				str = "QUNS_QUIET_TIME"; break;
		case QUNS_APP:						str = "QUNS_APP"; break;
	}

	return str;
}

void WatchUserNotificationState()
{
	HMODULE hLibrary = ::LoadLibrary(L"shell32.dll");
	if (hLibrary == NULL)
	{
		std::cerr << "LoadLibrary() failed." << std::endl;
		return;
	}

	PSHQueryUserNotificationState pSHQueryUserNotificationState;
	pSHQueryUserNotificationState = (PSHQueryUserNotificationState)(::GetProcAddress(hLibrary, "SHQueryUserNotificationState"));
	if (pSHQueryUserNotificationState == NULL)
	{
		std::cerr << "GetProcAddress() failed." << std::endl;
		return;
	}

	QUERY_USER_NOTIFICATION_STATE quns;
	HRESULT res;
	std::string str;

	while (true)
	{
		res = pSHQueryUserNotificationState(&quns);

		std::cout << ToString(quns) << std::endl;

		::Sleep(500);
	}

	::FreeLibrary(hLibrary);
}

void WatchQuietHoursState()
{
	static constexpr wchar_t * REG_SUB_KEY_NOTIFICATIONS_SETTINGS = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Notifications\\Settings";
	static constexpr wchar_t * REG_VALUE_TOAST_ENABLED = L"NOC_GLOBAL_SETTING_TOASTS_ENABLED";

	HKEY hRegistryKey = NULL;
	DWORD dwType = 0;
	DWORD dwData = 0;
	DWORD dwDataSize = 0;

	bool quietHours;

	if (::RegOpenKeyEx(HKEY_CURRENT_USER, REG_SUB_KEY_NOTIFICATIONS_SETTINGS, NULL, KEY_READ, &hRegistryKey) != ERROR_SUCCESS)
	{
		std::cerr << "Opening registry key failed." << std::endl;
		return;
	}

	while (true)
	{
		if (::RegGetValue(hRegistryKey, NULL, REG_VALUE_TOAST_ENABLED, RRF_RT_REG_DWORD, &dwType, &dwData, &dwDataSize) != ERROR_SUCCESS)
		{
			std::cout << "Quiet hours are inactive." << std::endl;
		}
		else
		{
			std::cout << "Quiet hours are active." << std::endl;
		}

		::Sleep(500);
	}
}

int main()
{
	EWatch watch = EWatch::USER_NOTIFICATION_STATE;

	switch (watch)
	{
		case EWatch::USER_NOTIFICATION_STATE:
		{
			WatchUserNotificationState();
			break;
		}

		case EWatch::USER_QUIET_HOURS:
		{
			WatchQuietHoursState();
			break;
		}
	}

    return 0;
}

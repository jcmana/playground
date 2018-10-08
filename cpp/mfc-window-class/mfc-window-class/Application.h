#pragma once

#include <sdkddkver.h>
#include <afxwin.h>
#include <string>

#include "WindowProc.h"
#include "NotificationArea.h"

class CApplication : public CWinApp
{
	public:
		CApplication();

		BOOL InitInstance();

	public:
		std::wstring		m_wsWindowClassName;
		std::wstring		m_wsWindowName;

		CNotificationArea	m_notificationArea;
		CNotificationArea	m_notificationArea2;
};

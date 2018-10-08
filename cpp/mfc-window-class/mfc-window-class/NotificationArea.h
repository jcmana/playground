#pragma once

#include <sdkddkver.h>
#include <windows.h>
#include <string>
#include <iostream>

#include "resource.h"

class CNotificationArea
{
public:
	CNotificationArea();

	void Initialize(HWND windowHandle, UINT uID);
	void Release();

	void CreateIcon();
	void DeleteIcon();

	void ShowIcon();
	void HideIcon();

	void SetTooltip(std::wstring tooltip);

private:
	HWND			m_windowHandle;
	UINT			m_uID;
	HICON			m_iconHandle;
	std::wstring	m_tooltipText;
};

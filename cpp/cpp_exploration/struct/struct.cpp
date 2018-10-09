#include <windows.h>
#include <string>

struct S_InitDescriptor
{
	HINSTANCE			m_hInstance = nullptr;
	HWND				m_ParentHwnd = nullptr;
	UINT				m_CallbackMsg = 0;
	HICON				m_hIcon = nullptr;
	UINT				m_ResourceID = 0;

	std::wstring		m_WindowName;
	std::wstring		m_WindowClassName;
	std::wstring		m_StrTooltip;

	bool				m_Hidden = false;
	bool				m_Invisible = false;

	std::uint32_t		m_ActionCommandID = 0;

	std::uint32_t		m_ShowIcoTimerUID = 0;
	std::uint32_t		m_AnimIcoTimerUID = 0;
	std::uint32_t		m_NgSetLastInputTimeTimerUID = 0;
	std::uint32_t		m_IcoPollingTimerUID = 0;
	std::uint32_t		m_AnimationStep = 0;
};


int main()
{
    return 0;
}


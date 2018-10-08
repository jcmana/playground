#include <iostream>
#include <string>
#include <memory>
#include <thread>

#include "../mfc-window-class/DesktopWindowClass.h"
#include "../mfc-window-class/DesktopWindow.h"
#include "../mfc-window-class/DesktopWindowMessageDef.h"


class CHiddenDesktopWindow : public asw::winapi::CDesktopWindow
{
public:
	class Factory
	{
	public:
		std::shared_ptr<IDesktopWindow> CHiddenDesktopWindow::Factory::CreateHiddenDesktopWindow() const
		{
			std::shared_ptr<IDesktopWindow> spImpl = std::make_shared<CHiddenDesktopWindow>();
			return spImpl;
		}
	};

	virtual bool OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override
	{
		std::wcout << L"OnMessage() for window: ";
		std::wcout << m_wsWindowName;
		std::wcout << std::endl;

		if (uMsg == WM_MFCWINDOW_MESSAGE)
		{
			std::wcout << L"    ";
			std::wcout << (wchar_t *)(lParam);
			std::wcout << std::endl;
		}

		if (uMsg == WM_QUIT)
		{
			std::wcout << L"    quit received";
			std::wcout << std::endl;

			::PostQuitMessage(0);
		}

		return true;
	}
};


class Runner :
	public asw::winapi::CDesktopWindowClass::Factory,
	public asw::winapi::CDesktopWindow::Factory,
	public CHiddenDesktopWindow::Factory
{
public:
	void TestMFCWindow()
	{
		/* Window Class wc1 */
		std::wstring wc1_className = L"asw_window_class";
		std::shared_ptr<asw::winapi::IDesktopWindowClass> wc1 = CreateDesktopWindowClass();
		wc1->SetClassName(wc1_className);
		if (wc1->Register() == false)
		{
			std::cout << "Window class registreation failed" << std::endl;
		}

		std::shared_ptr<asw::winapi::IDesktopWindow> w1 = CreateDesktopWindow();
		w1->SetClassName(wc1_className);
		w1->SetWindowName(L"window one");
		if (w1->Create() == false)
		{
			std::cout << "Window create failed" << std::endl;
		}

		std::shared_ptr<asw::winapi::IDesktopWindow> w2 = CreateDesktopWindow();
		w2->SetClassName(wc1_className);
		w2->SetWindowName(L"window two");
		w2->SetParent(w1->GetSafeHwnd());
		if (w2->Create() == false)
		{
			std::cout << "Window create failed" << std::endl;
		}


		/* Window Class wc2 */
		std::wstring wc2_className = L"asw_window_class_for_something_else";
		std::shared_ptr<asw::winapi::IDesktopWindowClass> wc2 = CreateDesktopWindowClass();
		wc2->SetClassName(wc2_className);
		if (wc2->Register() == false)
		{
			std::cout << "Window class registreation failed" << std::endl;
		}

		std::shared_ptr<asw::winapi::IDesktopWindow> w3 = CreateDesktopWindow();
		w3->SetClassName(wc2_className);
		w3->SetWindowName(L"window three");
		if (w3->Create() == false)
		{
			std::cout << "Window create failed" << std::endl;
		}


		/* Testing thread */
		std::thread testingThread = std::thread([]
		{
			HWND hWnd = ::FindWindow(L"asw_window_class", NULL);

			for (int n = 0; n < 5; ++n)
			{
				if (hWnd)
				{
					::SendMessageTimeout(hWnd, WM_MFCWINDOW_MESSAGE, NULL, (LPARAM)(L"zprava neasi"), SMTO_NORMAL, 500, NULL);
				}

				::Sleep(1500);
			}

			::SendMessageTimeout(hWnd, WM_QUIT, NULL, NULL, SMTO_NORMAL, 500, NULL);
		});


		/* Application message loop */

		HWND hWnd;
		MSG msg;

		while (::GetMessage(&msg, NULL, NULL, NULL))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// Wait for testing thread exit
		testingThread.join();

		return;
	}

	void TestMFCWindowSpecialized()
	{
		std::shared_ptr<asw::winapi::IDesktopWindowClass> spDesktopWindowClass = CreateDesktopWindowClass();
		spDesktopWindowClass->SetClassName(L"jebex_pica");
		spDesktopWindowClass->SetStyle(NULL);
		if (spDesktopWindowClass->Register() == false)
		{
			std::cout << "Window class registration failed" << std::endl;
		}

		std::shared_ptr<asw::winapi::IDesktopWindow> spHiddenDesktopWindow = CreateHiddenDesktopWindow();
		spHiddenDesktopWindow->SetClassName(L"jebex_pica");
		spHiddenDesktopWindow->SetWindowName(L"Pojebex pica");
		if (spHiddenDesktopWindow->Create() == false)
		{
			std::cout << "Window create failed" << std::endl;
		}

		/* Testing thread */
		std::thread testingThread = std::thread([]
		{
			HWND hWnd = ::FindWindow(L"jebex_pica", NULL);

			for (int n = 0; n < 5; ++n)
			{
				if (hWnd)
				{
					::SendMessageTimeout(hWnd, WM_MFCWINDOW_MESSAGE, NULL, (LPARAM)(L"zprava neasi"), SMTO_NORMAL, 500, NULL);
				}

				::Sleep(1500);
			}

			::SendMessageTimeout(hWnd, WM_QUIT, NULL, NULL, SMTO_NORMAL, 500, NULL);
		});


		/* Application message loop */
		HWND hWnd;
		MSG msg;

		while (::GetMessage(&msg, NULL, NULL, NULL))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// Wait for testing thread to exit
		testingThread.join();
		return;
	}
};

int main(void)
{
	//Runner().TestMFCWindow();
	Runner().TestMFCWindowSpecialized();

	return 0;
}
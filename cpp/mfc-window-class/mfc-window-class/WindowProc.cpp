#include "WindowProc.h"

LRESULT ApplicationWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COPYDATA:
			return 0;

		case WM_TIMER:
			std::cout << "fuckin timer" << std::endl;
			return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
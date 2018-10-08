#include <windows.h>

#include "WindowMessageDistributor.h"

namespace internal {

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

} // namespace detail
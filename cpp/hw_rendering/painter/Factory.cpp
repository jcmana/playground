#include <exception>
#include <d2d1.h>
#include <d2d1_1.h>

#include "RenderTarget.h"
#include "Factory.h"

namespace {

template<class Interface>
inline void SafeRelease(Interface ** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = nullptr;
	}
}

} // namespace


namespace D2 {

Factory::Factory()
{
	HRESULT hRes;

	hRes = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2Factory);
	if (FAILED(hRes))
	{
		m_D2Factory = nullptr;
		throw std::exception("Failed to initialize Direct2D factory.");
	}
}

Factory::~Factory()
{
	SafeRelease(&m_D2Factory);
}

void Factory::CreateHwndRenderTarget(HWND hWnd, int widht, int height)
{
}

} // namespace D2

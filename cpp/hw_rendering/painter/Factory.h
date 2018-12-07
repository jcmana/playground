#pragma once

struct ID2D1Factory;
struct ID2D1RenderTarget;

namespace D2 {


class Factory
{
public:
	Factory();
	~Factory();

	void CreateHwndRenderTarget(HWND hWnd, int widht, int height);

private:
	ID2D1Factory * m_D2Factory = nullptr;
};


} // namespace D2
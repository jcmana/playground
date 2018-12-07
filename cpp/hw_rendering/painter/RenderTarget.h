#pragma once

struct ID2D1RenderTarget;

namespace D2 {

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

private:
	ID2D1RenderTarget * m_D2RenderTarget = nullptr;
};

} // namespace D2
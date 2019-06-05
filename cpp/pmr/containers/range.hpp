#pragma once

namespace pmr {
namespace containers {

template<typename T, typename I>
class range
{
public:
	virtual ~range()= default;

	virtual I begin() = 0;
	virtual I end() = 0;
};

} // namespace containers
} // namespace pmr

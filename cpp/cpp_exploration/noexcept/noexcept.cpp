#include <iostream>
#include <exception>

void throwing_function() noexcept(false)
{
	throw std::exception("fuck you");
}

void noexcept_function() noexcept(true)
{
	throwing_function();
}

int main()
{
	noexcept_function();

	// conclusion: FUCKING BULLSHIT. UNUSABLE PIECE OF SHIT FEATURE!!! DESIGNED BY MORONS!

	std::getchar(); return 0;
};
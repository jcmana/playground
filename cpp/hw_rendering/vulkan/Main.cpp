#include <exception>

#include "Application.h"

int main()
{
	Application app;

	try
	{
		app.run();
	}
	catch (const std::exception & e)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
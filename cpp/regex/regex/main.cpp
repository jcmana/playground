#include <regex>
#include <iostream>

int main()
{
	std::string	path("C:\\eicar\\eicar.com");
	std::regex	re("([^\\\\:*?<>|]+)$");
	std::smatch match;

	if (std::regex_search(path, match, re))
	{
		std::cout << match[1] << std::endl;
	}

	return 0;
}
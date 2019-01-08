#include <iostream>
#include <string>


int main()
{
	// check string for prefix:
	if (false)
	{
		std::string str("scene_ref_1");
		std::string str_prefix("scene_");

		if (str_prefix.compare(0, str_prefix.size(), str) != 0)
		{
			std::cout << "string '" << str << "' has prefix '" << str_prefix << "'" << std::endl;
		}
	}

	// capacity():
	if (true)
	{
		std::string str;
		std::cout << str.capacity() << std::endl;

		for (int n = 0; n < 30; ++n)
		{
			str.push_back('a');
			std::cout << str.capacity() << std::endl;
		}
	}

	std::getchar(); return 0;
}
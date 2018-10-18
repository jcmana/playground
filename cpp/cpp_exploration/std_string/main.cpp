#include <iostream>
#include <string>


int main()
{
	// check string for prefix:
	{
		std::string str("scene_ref_1");
		std::string str_prefix("scene_");

		if (str_prefix.compare(0, str_prefix.size(), str) != 0)
		{
			std::cout << "string '" << str << "' has prefix '" << str_prefix << "'" << std::endl;
		}
	}

	return 0;
}
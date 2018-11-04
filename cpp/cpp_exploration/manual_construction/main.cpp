#include <cstdlib>

#include <iostream>
#include <string>
#include <memory>

struct custom_construction
{
	custom_construction(const std::string & name) :
		m_name(name)
	{
		m_value = 16;
	};

	std::string m_name;
	int m_value;
};

int main()
{
	// allocate memory for the class
	custom_construction * cc_ptr = (custom_construction *)(malloc(sizeof(custom_construction)));

	// call the constructor manually (:: is required here)
	cc_ptr->custom_construction::custom_construction("abc");

	std::cout << cc_ptr->m_name << " : " << cc_ptr->m_value << std::endl;

	return 0;
}
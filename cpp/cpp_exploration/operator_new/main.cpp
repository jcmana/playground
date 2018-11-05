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

	// operator new: memory allocation for requested size
	void * operator new(std::size_t count)
	{
		std::cout << "custom_construction::operator new()" << std::endl;
		return std::malloc(count);
	}

	// placement operator new: in-place construction without memory allocation
	void * operator new(std::size_t count, void * ptr)
	{
		std::cout << "custom_construction::operator new() (with placement)" << std::endl;
		return ptr;
	}

	std::string m_name;
	int m_value;
};

int main()
{
	// allocate memory for the class
	//custom_construction * cc_ptr = nullptr;
	custom_construction * cc_ptr = (custom_construction *)(std::malloc(sizeof(custom_construction)));

	// #1 call the constructor manually (:: is required here)
	// (warning: this might be Visual C++ specific thing and is not guaranteed by the standard)
	//cc_ptr->custom_construction::custom_construction("abc");

	// #2 call the constructor manually (calling "placement new")
	auto * ptr = new(cc_ptr) custom_construction("abc");

	std::cout << cc_ptr->m_name << " : " << cc_ptr->m_value << std::endl;

	std::getchar(); return 0;
}
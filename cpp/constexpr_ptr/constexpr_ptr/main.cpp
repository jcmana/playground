#include <iostream>

constexpr char * ptr = "kokot";
constexpr char arr[] = "picus";

int main()
{
	// no modification
	std::cout << ptr << std::endl;
	std::cout << arr << std::endl;

	// attempt to modify memory
	ptr[2] = 'r';		// runtime error: Access violation.
	//arr[2] = 'v';		// sytax error: 'arr': you cannot assign to a variable that is const.

	std::cout << ptr << std::endl;
	std::cout << arr << std::endl;

	return 0;
}
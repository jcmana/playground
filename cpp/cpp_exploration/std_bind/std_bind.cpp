#include <iostream>
#include <functional>

void function(int val, int & ref, int * ptr)
{
    std::cout << "function()" << "\n";
    std::cout << val << "\n";
    std::cout << ref << "\n";
    std::cout << *ptr << "\n";
}
void main()
{
    int val = 7;
    auto b = std::bind(function, val, val, &val);
    val = 42;
    b();
}
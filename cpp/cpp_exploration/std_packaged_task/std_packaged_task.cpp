#include <iostream>
#include <future>

void function(int value)
{
    std::cout << "function(" << value << ")" << std::endl;
}

void main()
{
    std::packaged_task<void(int)> p(function);
    p(4);
}
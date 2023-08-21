#include <iostream>

void function()
{
    std::cout << "function()" << std::endl;
}

int main()
{
    auto x = function;
}
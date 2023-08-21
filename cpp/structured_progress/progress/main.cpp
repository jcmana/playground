#include <iostream>

#include "structured_progress.hpp"

int main()
{
    structured_progress p;

    std::cout << p.percentage() << std::endl;

    p.parts.emplace_back(8);
    p.parts.emplace_back(2);

    std::cout << p.percentage() << std::endl;
    p.parts[0].value++;
    std::cout << p.percentage() << std::endl;
    p.parts[1].value++;
    std::cout << p.percentage() << std::endl;
}
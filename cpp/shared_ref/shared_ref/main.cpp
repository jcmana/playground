#include <iostream>

#include "shared_ref.hpp"

void main()
{
    shared_ref<int> sr = std::make_shared<int>(7);
    shared_ref<int> sr_copy = sr;

    (*sr)++;
    (*sr_copy)++;

    std::cout << (*sr) << std::endl;
    std::cout << (*sr_copy) << std::endl;

    //sr = std::make_shared<int>(4);

    std::shared_ptr<int> sp = sr;
    (*sp)++;

    std::cout << (*sr) << std::endl;
    std::cout << (*sr_copy) << std::endl;
    std::cout << (*sp) << std::endl;
}
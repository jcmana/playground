#include <iostream>
#include <string>

#include "shared_ref.hpp"

void main()
{
    {
        shared_ref<int> sr = 7;
        shared_ref<int> sr_copy = sr;

        (*sr)++;
        (*sr_copy)++;

        std::cout << (*sr) << std::endl;
        std::cout << (*sr_copy) << std::endl;

        //sr = 4;        // compile error, deleted function

        std::shared_ptr<int> sp = sr;
        (*sp)++;

        std::cout << (*sr) << std::endl;
        std::cout << (*sr_copy) << std::endl;
        std::cout << (*sp) << std::endl;
    }

    {
        const shared_ref<int> sr = 7;
        (*sr)++;
    }

    {
        shared_ref<const int> sr = 7;
        //(*sr)++;      // compile error, const expression
    }

    {
        shared_ref<std::string> sr = "asdfasdf";
        std::cout << (*sr) << std::endl;
    }

    // Construct from a copy
    {
        std::string s = "tuhnhdfgh";
        shared_ref<std::string> sr = s;
    }

    // Construct from a rref
    {
        shared_ref<std::string> sr = std::string("tuhnhdfgh");
    }
}
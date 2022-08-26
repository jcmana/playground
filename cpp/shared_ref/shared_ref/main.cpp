#include <iostream>
#include <string>

#include "unique_ref.hpp"
#include "shared_ref.hpp"

shared_ref<int> return_shared_ref()
{
    shared_ref<int> sr(0);
    (*sr) = 7;
    return sr;
}

int main()
{
    {
        shared_ref<int> sr(7);
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

        auto sp_ptr = sp.get();
    }

    {
        const shared_ref<int> sr(7);
        (*sr)++;
    }

    {
        shared_ref<const int> sr(7);
        //(*sr)++;      // compile error, const expression
    }

    {
        shared_ref<std::string> sr("asdfasdf");
        std::cout << (*sr) << std::endl;
    }

    // Missing default ctor
    {
        //shared_ref<int> sr;
    }

    // Construct from a copy
    {
        std::string s = "tuhnhdfgh";
        shared_ref<std::string> sr(s);
    }

    // Construct from a rref
    {
        std::string s("tuhnhdfgh");
        shared_ref<std::string> sr(std::move(s));
    }

    // Construct from a shared_ptr
    {
        auto sp = std::make_shared<std::string>("xvcbjteyueetu");
        auto sr = shared_ref(sp);
        if (true) auto sq = shared_ref<int>(std::shared_ptr<int>());        // runtime exception
    }

    {
        unique_ref<int> ur = 7;
        //unique_ref<int> ur_copy = ur;             // compile error, deleted function
        //unique_ref<int> ur_move = std::move(ur);  // compile error, deleted function
    }

    // Return shared from a function
    {
        auto sr = return_shared_ref();
    }

    // From nullptr:
    {
        shared_ref<int> sr(nullptr);
    }
}
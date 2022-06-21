#include <iostream>
#include <string>

#include "unique_ref.hpp"
#include "shared_ref.hpp"

struct T
{
    int value = 0;

    T()
    {
        std::cout << "T::T()" << std::endl;
    }

    T(const T & other)
    {
        std::cout << "T::T(const T &)" << std::endl;
    }

    T(T && other) = delete;

    T & operator  =(const T & other)
    {
        std::cout << "T::T(T &&)" << std::endl;
    }

    T & operator  =(T && other) = delete;
};

T nonmovable_return_test()
{
    T local;
    local.value = 7;
    return T(local);
}

shared_ref<int> return_shared_ref()
{
    shared_ref<int> sr;
    (*sr) = 7;
    return decltype(sr)(sr);
}

int main()
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

        auto sp_ptr = sp.get();
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
        std::string s("tuhnhdfgh");
        shared_ref<std::string> sr = std::move(s);
    }

    // Construct from a shared_ptr
    {
        auto sp = std::make_shared<std::string>("xvcbjteyueetu");
        auto sr = from_shared_ptr(sp);
        if (false) auto sq = from_shared_ptr<std::string>(nullptr);        // runtime exception
    }

    {
        unique_ref<int> ur = 7;
        //unique_ref<int> ur_copy = ur;             // compile error, deleted function
        //unique_ref<int> ur_move = std::move(ur);  // compile error, deleted function
    }
}
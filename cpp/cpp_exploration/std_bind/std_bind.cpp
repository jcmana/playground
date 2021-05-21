#include <iostream>
#include <functional>
#include <type_traits>
#include <tuple>

void function()
{
    std::cout << "function()" << std::endl;
}

/*
void function(int val, int & ref, int * ptr)
{
    std::cout << "function()" << "\n";
    std::cout << val << "\n";
    std::cout << ref << "\n";
    std::cout << *ptr << "\n";
}
*/

struct callback
{
    void method() const
    {
        std::cout << "callback::method()" << std::endl;
    }
};

template<typename ... A>
void call(A ... args);

template<typename F, typename ... A>
void call(F && f,  A ... args)
{
    return (f)(args ...);
}

template<typename F, typename T, typename ... A>
void call(F && f, T & t, A ... args)
{
    return (t.*f)(args ...);
}

template<typename ... A>
auto partial_call(A ... args)
{
    return std::tuple(args ...);
}

void main()
{
    // Full application:
    if (false)
    {
        /*
        int val = 7;
        auto b = std::bind(function, val, val, &val);
        val = 42;
        b();
        */
    }

    if (false)
    {
        call(function);
        callback c;
        call(&callback::method, c);
    }

    if (true)
    {
        using namespace std::placeholders;

        callback c;
        auto pc = partial_call(_1, c);
    }
}
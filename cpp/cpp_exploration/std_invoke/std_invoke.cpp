#include <iostream>
#include <thread>
#include <functional>

struct callback
{
    void method()
    {
        std::cout << "callback_intf::method(): " << std::endl;
    }

    void method_slow()
    {
        std::cout << "callback_intf::method_slow()" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

struct callback_listener
{
    template<typename F, typename ... A>
    void operator ()(F && method, A ... args)
    {
    }
};

void main()
{
    // Invoking method
    callback cb;
    std::invoke(&callback::method, &cb);

    // Invoking functor
    callback_listener cbl;
    std::invoke(cbl, &callback::method, &cb);
}
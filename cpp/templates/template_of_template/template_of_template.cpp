#include <iostream>


#include <functional>
#include <utility>
#include <type_traits>


template<typename T, typename E = void>
struct invocation;

template<typename T>
struct invocation<T, typename std::enable_if_t<std::is_function_v<std::remove_pointer_t<T>>>>
{
    invocation(T object) :
        m_object(object)
    {
    }

    template<typename ... A>
    void operator ()(A ... args)
    {
        if constexpr (std::is_function_v<std::remove_pointer_t<T>>)
        {
            m_object(std::forward<A>(args) ...);
        }

        if constexpr (std::is_class_v<T>)
        {
            m_object(std::forward<A>(args) ...);
        }
    }

    template<typename ... A, typename = std::enable_if_t<std::is_class_v<T>>>
    void operator ()(void(T:: * method)(A ...), A ... args)
    {
        (m_object.*method)(std::forward<A>(args) ...);
    }

    T m_object;
};

template<typename T, typename I = invocation<T>>
struct callback
{
    callback(T object) :
        m_invocation(object)
    {
    }

    template<typename ... A>
    void operator ()(A ... args)
    {
        m_invocation(std::forward<A>(args) ...);
    }

    I m_invocation;
};

void function()
{
    std::cout << "function()" << std::endl;
}

struct callback_interface
{
    void method()
    {
        std::cout << "callback_interface::method()" << std::endl;
    }
};

void main()
{
    {
        callback_interface ci;

        callback cb(std::ref(ci));
        cb(&callback_interface::method);
    }

    {
        auto l = []
        {
            std::cout << "[] void ()" << std::endl;
        };

        callback cb(std::move(l));
        cb();
    }

    {
        //callback cb(function);
        //cb();
    }
}
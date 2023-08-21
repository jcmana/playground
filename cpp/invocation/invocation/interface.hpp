#pragma once

namespace invocation
{

template<typename T>
class interface
{
public:
    interface() noexcept :
        m_interface_ptr(nullptr)
    {
    }

    interface(T & interface_ref) noexcept :
        m_interface_ptr(&interface_ref)
    {
    }

    interface(const interface<T> & other) noexcept :
        m_interface_ptr(other.m_interface_ptr)
    {
    }

    interface(interface<T> && other) noexcept :
        m_interface_ptr(other.m_interface_ptr)
    {
        other.m_interface_ptr = nullptr;
    }

    auto & operator  =(const interface<T> & other) noexcept
    {
        m_interface_ptr = other.m_interface_ptr;
        return (*this);
    }

    auto & operator  =(interface<T> && other) noexcept
    {
        m_interface_ptr = other.m_interface_ptr;
        other.m_interface_ptr = nullptr;
        return (*this);
    }

    template<typename R, typename ... A>
    auto operator ()(R(T:: * method_ptr)(A ...), A ... args)
    {
        return (*m_interface_ptr.*method_ptr)(std::forward<A>(args) ...);
    }

private:
    T * m_interface_ptr;
};

} // namespace invocation

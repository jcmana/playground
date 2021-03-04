#pragma once

/// \brief      Callable reference to user-type object.
///
/// Stores object reference on which any method can be invoked using `operator()`. This 
/// is helper type for `callback` class, easing `callback` creation for some cases.
template<typename T>
class callback_ref
{
public:
    callback_ref() noexcept :
        m_object_ptr(nullptr)
    {
    }

    callback_ref(T & object_ref) noexcept :
        m_object_ptr(&object_ref)
    {
    }

    /*
    callback_ref(const callback_ref<T> & other) noexcept :
        m_object_ptr(other.m_object_ptr)
    {
    }

    callback_ref(callback_ref<T> && other) noexcept :
        m_object_ptr(other.m_object_ptr)
    {
        other.m_object_ptr = nullptr;
    }

    auto & operator  =(const callback_ref<T> & other) noexcept
    {
        m_object_ptr = other.m_object_ptr;
        return (*this);
    }

    auto & operator  =(callback_ref<T> && other) noexcept
    {
        m_object_ptr = other.m_object_ptr;
        other.m_object_ptr = nullptr;
        return (*this);
    }
    */

    template<typename R, typename ... A>
    auto operator ()(R(T:: * method_ptr)(A ...), A ... args)
    {
        return (*m_object_ptr.*method_ptr)(std::forward<A>(args) ...);
    }

private:
    T * m_object_ptr;
};
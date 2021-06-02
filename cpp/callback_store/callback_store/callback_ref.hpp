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

    template<typename R, typename ... A>
    auto operator ()(R(T:: * method_ptr)(A ...), A ... args) const
    {
        return (*m_object_ptr.*method_ptr)(std::forward<A>(args) ...);
    }

private:
    T * m_object_ptr;
};
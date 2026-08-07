#pragma once

#include <concepts>

template <typename T, typename S>
concept Transactional = requires(S & system, T * ptr)
{
    { system.lock(ptr) } -> std::same_as<void>;
};

/// \brief      Transaction providing interface `T` on system `S`.
template<typename T, typename S> /*requires Transactional<T, S>*/
class basic_txn
{
public:
    basic_txn(S & system_ref) :
        m_system_ptr(&system_ref)
    {
        m_system_ptr->lock(m_system_api_ptr);
    }

    ~basic_txn()
    {
        m_system_ptr->unlock(m_system_api_ptr);
    }

    T & get()
    {
        return (*m_system_api_ptr);
    }

    T * operator ->()
    {
        return m_system_api_ptr;
    }

private:
    S * m_system_ptr;
    T * m_system_api_ptr;
};
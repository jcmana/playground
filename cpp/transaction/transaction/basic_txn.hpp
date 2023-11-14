#pragma once

#include <string>

/// \brief      Transactional system.
/// \note       Only for illustration, not a reusable implementataion.
class basic_txn_system
{
public:
    struct shared
    {
    };

    struct unique : shared
    {
    };

public:
    void lock()
    {
    }

    bool try_lock()
    {
        return true;
    }

    void unlock()
    {
    }

private:
    struct base
    {
        std::string text;
    };

private:
    struct base m_base;
    struct shared m_shared;
};

/// \brief      Transaction of type `T` on system `S`.
template<typename T, typename S>
class basic_txn : public T
{
public:
    basic_txn() :
        m_system_ptr(nullptr),
        m_system_api_ptr(nullptr)
    {
    }

    basic_txn(S & system_ref) :
        m_system_ptr(&system_ref)
{
        m_system_ptr->lock();
    }

    ~basic_txn()
    {
        if (m_system_ptr)
        {
            m_system_ptr->unlock();
        }
    }

private:
    S * m_system_ptr;
    T * m_system_api_ptr;
};
#include <iostream>
#include <mutex>

#include "basic_txn.hpp"

class txnsystem
{
public:
    struct shared
    {
        shared(txnsystem & ref) :
            m_ref(ref)
        {
        }

        txnsystem & m_ref;

        std::string read() const
        {
            return m_ref.m_text;
        }
    };

    struct unique : shared
    {
        unique(txnsystem & ref) :
            shared(ref),
            m_ref(ref)
        {
        }

        txnsystem & m_ref;

        void write(const std::string & text)
        {
            m_ref.m_text = text;
        }
    };

public:
    txnsystem() :
        m_shared{*this},
        m_unique{*this}
    {
    }

    // shared txn implementation:
    void lock(shared *& ptr)
    {
        ptr = &m_shared;
    }

    bool try_lock(shared *& ptr)
    {
        ptr = &m_shared;
        return true;
    }

    void unlock(shared *& ptr)
    {
    }

    // unique txn implementation:
    void lock(unique *& ptr)
    {
        ptr = &m_unique;
    }

    bool try_lock(unique *& ptr)
    {
        ptr = &m_unique;
        return true;
    }

    void unlock(unique *& ptr)
    {
    }

private:
    struct base
    {
        std::string text;
    };

private:
    std::string m_text;

    struct shared m_shared;
    struct unique m_unique;
};

int main()
{
    // Brainstorming:
    // - is it just unique and shared?
    // - operations: read, modify, rollback?
    // - target type API:
    //      - support for the operations
    //      - main concern from shared_obe+unique_txn: how to handle modification notification?


    // First kick-off
    txnsystem s;

    {
        auto txn = basic_txn<txnsystem::shared, txnsystem>(s);
        std::cout << "text = " << txn->read() << std::endl;
    }

    {
        auto txn = basic_txn<txnsystem::unique, txnsystem>(s);
        txn->write("asdfasdf");
    }

    {
        auto txn = basic_txn<txnsystem::shared, txnsystem>(s);
        std::cout << "text = " << txn->read() << std::endl;
    }
}
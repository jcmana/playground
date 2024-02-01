#include <iostream>
#include <mutex>

struct composite
{
    composite() :
        m_base(),
        m_constant(m_base),
        m_threadsafe(m_base)
    {
    }

    struct base
    {
        std::mutex m_mutex;
    };

    struct constant
    {
        constant(base & base_ref) :
            m_base_ref(base_ref)
        {
        }

        void print()
        {
            std::cout << "A::constant::print()" << std::endl;
        }

        base & m_base_ref;
    };

    struct threadsafe
    {
        threadsafe(base & base_ref) :
            m_base_ref(base_ref)
        {
        }

        void print()
        {
            std::cout << "A::threadsafe::print()" << std::endl;
        }

        base & m_base_ref;
    };

    struct constant & constant()
    {
        return m_constant;
    }

    struct threadsafe & threadsafe()
    {
        return m_threadsafe;
    }

    struct base m_base;
    struct constant m_constant;
    struct threadsafe m_threadsafe;
};

void consumer(struct composite::threadsafe & x)
{
    x.print();
}

int main()
{
    composite c;
    c.constant().print();

    consumer(c.threadsafe());

    return 0;
}
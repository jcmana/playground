#pragma once

#include "observable.hpp"

template<typename T>
class circular_break_logic
{
public:
    circular_break_logic(shared_obe<T> so_a, shared_obe<T> so_b) :
        m_so_a(so_a),
        m_so_b(so_b)
    {
        auto forward_a = [this](const T & value)
        {
            if (m_b_to_a)
            {
                return;
            }

            m_a_to_b = true;
            unique_txn{m_so_b} = value;
            m_a_to_b = false;
        };
        m_so_a.observe(forward_a);

        auto forward_b = [this](const T & value)
        {
            if (m_a_to_b)
            {
                return;
            }

            m_b_to_a = true;
            unique_txn{m_so_a} = value;
            m_b_to_a = false;
        };
        m_so_b.observe(forward_b);
    }

private:
    bool m_a_to_b;
    bool m_b_to_a;

    shared_obe<T> m_so_a;
    shared_obe<T> m_so_b;
};
#pragma once

#include "../cursors/postorder_edge_cursor.hpp"

namespace containers {
namespace graph {

template<typename G>
class postorder_edge_iterator
{
public:
    postorder_edge_iterator() :
        m_cursor()
    {
    }

    postorder_edge_iterator(typename G::node * node_ptr) :
        m_cursor(node_ptr)
    {
        while (m_cursor.valid() == false)
        {
            m_cursor.expand();
        }
    }

    postorder_edge_iterator & operator ++()
    {
        m_cursor.consume();

        while (true)
        {
            if (m_cursor.valid())
            {
                break;
            }
            
            m_cursor.expand();
        }

        return (*this);
    }

    typename G::edge * operator ->() const
    {
        return *m_cursor;
    }

    template<typename GG>
    friend bool operator ==(const postorder_edge_iterator<GG> & lhs, const postorder_edge_iterator<GG> & rhs);

    template<typename GG>
    friend bool operator !=(const postorder_edge_iterator<GG> & lhs, const postorder_edge_iterator<GG> & rhs);

private:
    postorder_edge_cursor<G> m_cursor;
};

template<typename G>
bool operator ==(const postorder_edge_iterator<G> & lhs, const postorder_edge_iterator<G> & rhs)
{
    return lhs.m_cursor == rhs.m_cursor;
}

template<typename G>
bool operator !=(const postorder_edge_iterator<G> & lhs, const postorder_edge_iterator<G> & rhs)
{
    return lhs.m_cursor != rhs.m_cursor;
}

} // namespace graph
} // namespace containers

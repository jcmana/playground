#pragma once

#include "../cursors/postorder_path_cursor.hpp"

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

    postorder_edge_iterator(typename G::edge * edge_ptr) :
        m_cursor(edge_ptr)
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
        return (*m_cursor);
    }

    template<typename G>
    friend bool operator ==(const postorder_edge_iterator<G> & lhs, const postorder_edge_iterator<G> & rhs);

    template<typename G>
    friend bool operator !=(const postorder_edge_iterator<G> & lhs, const postorder_edge_iterator<G> & rhs);

private:
    postorder_path_cursor<G> m_cursor;
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

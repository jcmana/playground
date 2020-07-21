#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief      `node_centric` iterator basis.
template<typename G>
class postorder_edge_cursor
{
public:
    postorder_edge_cursor()
    {
    }

    postorder_edge_cursor(typename G::node * node_ptr)
    {
        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack_down.push_back(*edge_it);
        }
    }

    void consume()
    {
        m_stack_up.pop_back();
        m_stack_down.pop_back();
    }

    bool valid()
    {
        if (m_stack_down.empty())
        {
            return false;
        }

        if (m_stack_up.empty())
        {
            return false;
        }

        return m_stack_down.back() == m_stack_up.back();
    }

    bool empty()
    {
        return m_stack_down.empty() && m_stack_up.empty();
    }

    void expand()
    {
        auto * edge_ptr = m_stack_down.back();
        auto * node_ptr = edge_ptr->target;

        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack_down.push_back(*edge_it);
        }

        m_stack_up.push_back(edge_ptr);
    }

    typename G::edge * operator  *() const
    {
        return m_stack_down.back();
    }

    typename G::edge * operator ->() const
    {
        return m_stack_down.back();
    }

    template<typename Graph>
    friend bool operator ==(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs);

    template<typename Graph>
    friend bool operator !=(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs);

private:
    std::vector<typename G::edge *> m_stack_up;
    std::vector<typename G::edge *> m_stack_down;
};

template<typename Graph>
bool operator ==(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs)
{
    return lhs.m_stack == rhs.m_stack;
}

template<typename Graph>
bool operator !=(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs)
{
    return lhs.m_stack != rhs.m_stack;
}


} // namespace containers
} // namespace graph

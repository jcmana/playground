#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief      `node_centric` postorderdering logic.
template<typename G>
class postorder_path_cursor
{
public:
    postorder_path_cursor()
    {
    }

    postorder_path_cursor(typename G::edge * edge_ptr)
    {
        m_stack_down.push_back(edge_ptr);
        m_stack_path.push_back(edge_ptr);

        auto * node_ptr = edge_ptr->target;

        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack_down.push_back(*edge_it);
        }
    }

    void consume()
    {
        m_stack_path.pop_back();
        m_stack_down.pop_back();
    }

    bool valid()
    {
        if (m_stack_down.empty())
        {
            return false;
        }

        if (m_stack_path.empty())
        {
            return false;
        }

        return m_stack_down.back() == m_stack_path.back();
    }

    bool empty()
    {
        return m_stack_down.empty() && m_stack_path.empty();
    }

    void expand()
    {
        auto * edge_ptr = m_stack_down.back();
        auto * node_ptr = edge_ptr->target;

        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack_down.push_back(*edge_it);
        }

        m_stack_path.push_back(edge_ptr);
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
    friend bool operator ==(const postorder_path_cursor<Graph> & lhs, const postorder_path_cursor<Graph> & rhs);

    template<typename Graph>
    friend bool operator !=(const postorder_path_cursor<Graph> & lhs, const postorder_path_cursor<Graph> & rhs);

private:
    // Path to the last expanded node
    std::vector<typename G::edge *> m_stack_path;

    // Node preorder stack
    std::vector<typename G::edge *> m_stack_down;
};

template<typename Graph>
bool operator ==(const postorder_path_cursor<Graph> & lhs, const postorder_path_cursor<Graph> & rhs)
{
    return lhs.m_stack_path == rhs.m_stack_path && lhs.m_stack_down == rhs.m_stack_down;
}

template<typename Graph>
bool operator !=(const postorder_path_cursor<Graph> & lhs, const postorder_path_cursor<Graph> & rhs)
{
    return !(lhs == rhs);
}


} // namespace containers
} // namespace graph

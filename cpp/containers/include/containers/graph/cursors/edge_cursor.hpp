#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief      `node_centric` iterator basis.
template<typename Graph>
class edge_cursor
{
public:
    edge_cursor()
    {
    }

    edge_cursor(typename Graph::node * node_ptr)
    {
        expand_internal(node_ptr);
    }

    void consume()
    {
        m_stack.pop_back();
    }

    std::size_t expand()
    {
        auto * edge_ptr = m_stack.back();
        auto * node_ptr = edge_ptr->target;

        return expand_internal(node_ptr);
    }

    std::size_t consume_and_expand()
    {
        auto * edge_ptr = m_stack.back();
        auto * node_ptr = edge_ptr->target;

        m_stack.pop_back();

        return expand_internal(node_ptr);
    }

    const typename Graph::edge * operator  *() const
    {
        return m_stack.back();
    }

    const typename Graph::edge * operator ->() const
    {
        return m_stack.back();
    }

    template<typename Graph>
    friend bool operator ==(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs);

    template<typename Graph>
    friend bool operator !=(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs);

private:
    std::size_t expand_internal(typename Graph::node * node_ptr)
    {
        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack.push_back(*edge_it);
        }

        return node_ptr->outgoing.size();
    }


private:
    std::vector<typename Graph::edge *> m_stack;
};

template<typename Graph>
bool operator ==(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs)
{
    return lhs.m_stack == rhs.m_stack;
}

template<typename Graph>
bool operator !=(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs)
{
    return lhs.m_stack != rhs.m_stack;
}

} // namespace containers
} // namespace graph

#pragma once

#include <vector>

namespace containers {
namespace graph {

/// \brief      `node_centric` iterator basis.
template<typename G>
class edge_cursor
{
public:
    edge_cursor()
    {
    }

    edge_cursor(typename G::node * node_ptr)
    {
        expand_internal(node_ptr);
    }

    typename G::edge * consume()
    {
        auto * edge_ptr = m_stack.back();
        m_stack.pop_back();
        return edge_ptr;
    }

    std::size_t expand()
    {
        const auto * edge_ptr = m_stack.back();
        const auto * node_ptr = edge_ptr->target;

        return expand_internal(node_ptr);
    }

    std::size_t consume_and_expand()
    {
        const auto * edge_ptr = m_stack.back();
        const auto * node_ptr = edge_ptr->target;

        m_stack.pop_back();

        return expand_internal(node_ptr);
    }

    const typename G::edge * operator  *() const
    {
        return m_stack.back();
    }

    const typename G::edge * operator ->() const
    {
        return m_stack.back();
    }

    /*
    edge_cursor()
    {
    }

    edge_cursor(typename G::node * node_ptr)
    {
        stack_node entry;
        entry.node_ptr = node_ptr;
        entry.edge_it = node_ptr->outgoing.begin();

        m_stack.push_back(entry);
    }

    void consume()
    {
        m_stack.pop_back();
    }

    void expand()
    {
        auto * node_ptr = m_stack.back().node_ptr;
        auto * edge_ptr = (*m_stack.back().edge_it);

        stack_node entry;
        entry.node_ptr = edge_ptr->target;
        entry.edge_it = edge_ptr->target->outgoing.begin();
        
        m_stack.push_back(entry);
    }

    const typename G::edge * operator ->() const
    {
        return (*m_stack.back().edge_it);
    }
    */

    template<typename Graph>
    friend bool operator ==(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs);

    template<typename Graph>
    friend bool operator !=(const edge_cursor<Graph> & lhs, const edge_cursor<Graph> & rhs);

private:
    /*
    struct stack_node
    {
        typename G::node * node_ptr;
        typename G::edge_container::iterator edge_it;
    };
    */

private:
    std::size_t expand_internal(const typename G::node * node_ptr)
    {
        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack.push_back(*edge_it);
        }

        return node_ptr->outgoing.size();
    }

private:
    std::vector<typename G::edge *> m_stack;
    /*
    std::vector<stack_node> m_stack;
    */
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

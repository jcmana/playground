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
        //expand(node_ptr);
        //m_stack_up.push_back(nullptr);
    }

    /*
    typename G::edge * consume()
    {
        auto * edge_ptr = m_stack_down.back();
        m_stack_down.pop_back();
        return edge_ptr;
    }
    */

    typename G::edge * consume()
    {
        auto * edge_ptr = m_stack_up.back();
        m_stack_up.pop_back();
        return edge_ptr;
    }

    /*
    void defer(typename G::edge * edge_ptr)
    {
        m_stack_up.push_back(edge_ptr);
    }
    */

    std::size_t expand(typename G::edge * edge_ptr)
    {
        auto * node_ptr = edge_ptr->target;

        for (auto edge_it = node_ptr->outgoing.rbegin(); edge_it < node_ptr->outgoing.rend(); ++edge_it)
        {
            m_stack_down.push_back(*edge_it);
        }

        m_stack_up.push_back(edge_ptr);

        return node_ptr->outgoing.size();
    }

    const typename G::edge * operator  *() const
    {
        //return m_stack_down.back();
    }

    const typename G::edge * operator ->() const
    {
        //return m_stack_down.back();
        return m_stack_up.back();
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
    friend bool operator ==(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs);

    template<typename Graph>
    friend bool operator !=(const postorder_edge_cursor<Graph> & lhs, const postorder_edge_cursor<Graph> & rhs);

    /*
    struct stack_node
    {
        typename G::node * node_ptr;
        typename G::edge_container::iterator edge_it;
    };
    */

private:
    std::vector<typename G::edge *> m_stack_up;
    std::vector<typename G::edge *> m_stack_down;
    /*
    std::vector<stack_node> m_stack;
    */
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

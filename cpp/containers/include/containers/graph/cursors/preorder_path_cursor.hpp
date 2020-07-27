#pragma once

#include <vector>

namespace containers {
namespace graph {

template<typename G>
class preorder_path_cursor
{
public:
    void consume()
    {
        m_stack_path.pop_back();
        m_stack_down.pop_back();
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


private:
    std::vector<typename G::edge *> m_stack_down;
    std::vector<typename G::edge *> m_stack_path;
};

} // namespace graph
} // namespace containers

#pragma once

#include <vector>

namespace containers {
namespace graph {

template<typename G>
class preoder_path_cursor
{
public:


private:
    std::vector<typename G::edge *> m_stack_down;
    std::vector<typename G::edge *> m_stack_path;
};

} // namespace graph
} // namespace containers

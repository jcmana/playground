#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../iterator_v2/my_iterator.hpp"
#include "../iterator_v2/iterator.hpp"

using namespace testing;
using namespace pmr::iterator;

class forward_iterator_facade_int
{
public:
    MOCK_METHOD0(copy, void());
};

TEST(my_forward_iterator, api)
{
    auto up = std::make_unique<my_iterator<int>>(0);

    auto it = iterator<int>(std::move(up));
    
    auto it_move = std::move(it);
    auto it_copy = it_move;
}

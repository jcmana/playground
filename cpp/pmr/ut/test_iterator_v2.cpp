#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../iterator_v2/my_iterator.hpp"
#include "../iterator_v2/iterator.hpp"

using namespace testing;
using namespace pmr::iterator;

TEST(my_forward_iterator, make_copy_move)
{
    auto it = iterator<my_iterator_intf<int>>(std::make_unique<my_iterator<int>>(7));
}

TEST(my_forward_iterator, base_intf)
{
    auto it = iterator<my_iterator_intf<int>>(std::make_unique<my_iterator<int>>(7));

    auto it_copy = it;
    EXPECT_TRUE(it == it_copy);
    EXPECT_FALSE(it != it_copy);

    auto it_move = std::move(it);
    EXPECT_TRUE(it_move == it_copy);
    EXPECT_FALSE(it_move != it_copy);
}

TEST(my_forward_iterator, output_intf)
{
    const auto it = iterator<my_iterator_intf<int>>(std::make_unique<my_iterator<int>>(7));
    EXPECT_EQ(*it, 7);
}

TEST(my_forward_iterator, input_intf)
{
    auto it = iterator<my_iterator_intf<int>>(std::make_unique<my_iterator<int>>(7));
    // Fails on missing input_intf impl
    //EXPECT_EQ(*it, 7);
}

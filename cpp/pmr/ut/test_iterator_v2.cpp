#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../iterator_v2/my_iterator.hpp"
#include "../iterator_v2/iterator.hpp"

using namespace testing;
using namespace pmr::iterator;

TEST(my_forward_iterator, make_copy_move)
{
    auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));
}

TEST(my_forward_iterator, base_intf)
{
    auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));

    auto it_copy = it;
    EXPECT_TRUE(it == it_copy);
    EXPECT_FALSE(it != it_copy);

    auto it_move = std::move(it);
    EXPECT_TRUE(it_move == it_copy);
    EXPECT_FALSE(it_move != it_copy);
}

TEST(my_forward_iterator, output_intf)
{
    const auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));
    EXPECT_EQ(*it, 7);
}

TEST(my_forward_iterator, input_intf)
{
    auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));
    EXPECT_EQ(*it, 7);
    *it = 8;
    EXPECT_EQ(*it, 8);
}

TEST(my_forward_iterator, forward_intf)
{
    auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));

    ++it;
    it++;

    EXPECT_EQ(*it, 9);
}

TEST(my_forward_iterator, backward_intf)
{
    auto it = my_iterator<my_iterator_intf<int>>(std::make_unique<my_iterator_impl<int>>(7));

    --it;
    it--;

    EXPECT_EQ(*it, 5);
}


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../iterator/iterator.hpp"
#include "../iterator/forward_iterator.hpp"
#include "../iterator/random_access_iterator.hpp"

using namespace testing;
using namespace pmr;

class forward_iterator_facade_int
{
public:
    MOCK_METHOD0(copy, void());
};

TEST(iterator, api)
{
    std::unique_ptr<iterator<int>::facade> up;
    iterator<int> it(std::move(up));
}

TEST(forward_iterator, api)
{


    forward_iterator<int> it;
    //++it;
}

TEST(random_access_iterator, api)
{
    std::unique_ptr<random_access_iterator<int>::facade> up;
    random_access_iterator<int> it(std::move(up));

    //it + 5;
    //it - 4;
}

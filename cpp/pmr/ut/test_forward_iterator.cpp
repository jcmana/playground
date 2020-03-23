#include "gtest/gtest.h"

#include "../containers/iterator.hpp"
#include "../containers/forward_iterator.hpp"
#include "../containers/random_access_iterator.hpp"

using namespace pmr;

TEST(iterator, api)
{
	std::unique_ptr<iterator<int>::facade> up;
	iterator<int> it(std::move(up));
}

TEST(forward_iterator, api) 
{
	forward_iterator<int> it;
	++it;
}

TEST(random_access_iterator, api)
{
	std::unique_ptr<random_access_iterator<int>::facade> up;
	random_access_iterator<int> it(std::move(up));

	it + 5;
}

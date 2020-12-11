#include "gtest/gtest.h"

#include "../iterator/iterator.hpp"
#include "../iterator/forward_iterator.hpp"
#include "../iterator/random_access_iterator.hpp"

using namespace pmr;

/*
template<typename T>
class iterator_facade_mock : 
	iterator<T>::facade
{
	virtual bool equal(const facade & other) const = 0;
	virtual std::unique_ptr<facade> copy() const = 0;
};
*/

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

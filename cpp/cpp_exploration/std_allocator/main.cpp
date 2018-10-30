#include <iostream>
#include <set>
#include <string>
#include <memory>
#include <functional>

class data
{
public:
	data(const std::string & name) :
		m_name(name)
	{
	}

	~data()
	{
		std::cout << "destructor for " << m_name << std::endl;
	}

public:
	std::string m_name;
};

bool operator < (const data & left, const data & right)
{
	return left.m_name < right.m_name;
}


template <typename T>
class deleting_allocator
{
public:
	using value_type = T;
	using size_type = std::size_t;

	using pointer = value_type *;

public:
	pointer allocate(size_type n)
	{
		return (new sizeof(value_type) * n);
	}

	void deallocate(pointer p, size_type n)
	{
		delete p;
	}
};

int main()
{
	// by value:
	if (false)
	{
		std::set<data> dataset;
//		dataset.emplace("a");
		dataset.insert(data("b"));
//		dataset.insert(std::move(data("c")));
	}


	// by pointer:
	if (true)
	{
		std::set<data *, std::less<data *>, std::allocator<data *>> dataset;
		//dataset.emplace(new data("a"));
		dataset.insert(new data("b"));
		//dataset.insert(std::move(new data("c")));
	}

	return 0;
}
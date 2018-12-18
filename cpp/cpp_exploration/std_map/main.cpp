#include <iostream>
#include <map>
#include <utility>

#include "defaultable.hpp"

class custom_key
{
public:
	custom_key(int value) :
		m_value(value)
	{
	}

public:
	int m_value;
};

bool operator <(const custom_key & left, const custom_key & right)
{
	return left.m_value < right.m_value;
}

int main()
{
	{
		std::map<custom_key, int> my_map;

		custom_key k(3);
		custom_key l(8);

		my_map.insert(std::make_pair(k, 16));	// requires custom_key::operator <()
		int v = my_map.at(k);					// requires custom_key::operator <()
	}

	{
		defaultable_map<char, int, -1> map;
		map['a'];
		std::cout << map['a'] << std::endl;
	}

	return 0;
}
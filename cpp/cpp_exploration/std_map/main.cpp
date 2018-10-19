#include <map>
#include <utility>

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
	std::map<custom_key, int> my_map;
	
	custom_key k(3);

	my_map.insert(std::make_pair(k, 16));	// requires operator >()

	auto v = my_map.at(k);					// requires operator >()

	return 0;
}
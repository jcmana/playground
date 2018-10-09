#include <cstdint>

#include <iostream>
#include <initializer_list>
#include <vector>

class initializable
{
public:
	initializable(int a, int b) :
		m_a(a),
		m_b(b)
	{
	}

private:
	int m_a, m_b;
};


struct data
{
	int a;
	int b;
};

class initializable_nested
{
public:
	initializable_nested(int a, const initializable & i, const data & d) :
		m_a(a),
		m_i(i),
		m_d(d)
	{
	}

private:
	int m_a;
	initializable m_i;
	data m_d;
};


int main()
{
	// class list initialization:
	{
		// single-level list
		initializable i{ 1, 2 };

		// nested list
		initializable_nested in{ 1, {2, 3}, {3, 4} };

		// constructor call with combination of lists and values
		initializable_nested in2(1, { 2, 3 }, data{ 7, 8 });
	}

	{
		auto autolist = { 1, 2, 3 };	// auto deducted type = std::initializer_list<int>

		//initializable{ autolist };	// std::initializer_list cannot be further used for list initialization
										// if no constructor is defined to accept it
	}

	std::vector<char> v { std::vector<char>::size_type { 5 } };		// std::initializer_list have always priority if defined

	std::getchar();

	return 0;
}
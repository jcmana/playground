#include <iostream>

template <typename T>
class A
{
private:
	T x = 0;
	T y = 0;

template<typename T> friend bool operator ==(const A<T> & left, const A<T> & right);
};

template<typename T>
bool operator ==(const A<T> & left, const A<T> & right)
{
	return (left.x == right.x && left.y == right.y);
}

int main()
{
	A<int> m;
	A<int> n;

	bool x = (m == n);

	return 0;
}

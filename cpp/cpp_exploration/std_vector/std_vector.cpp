#include <vector>

class A
{
public:
	A(int * p) :
		pointer(p)
	{
	}

	~A()
	{
		delete pointer;
	}

private:
	int * pointer;
};

int main()
{ 
	std::vector<A> v;
	v.emplace_back(new int(7));

	v.reserve(256);

	return 0;
}
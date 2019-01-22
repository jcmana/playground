#include <iostream>
#include <atomic>

class my_class
{
public:
	unsigned int get() const
	{
		return m_value;
	}

	void reset()
	{
		m_value = 0;
	}

protected:
	unsigned int m_value;
};

int main()
{
	// primitive type atomic:
	{
		std::atomic<long> a;
		a;
		a = 6;
		a;
	}

	// custom type atomic:
	{
		std::atomic<my_class> a_my_class;
		a_my_class.load().reset();

		std::cout << a_my_class.load().get() << std::endl;
	}

	return 0;
}
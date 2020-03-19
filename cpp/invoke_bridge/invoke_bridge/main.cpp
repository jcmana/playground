#include <iostream>
#include <map>
#include <string>
#include <functional>

void a()
{
	std::cout << "a()" << std::endl;
}

void b()
{
	std::cout << "b()" << std::endl;
}

struct producer
{
	void x(int n)
	{
	}

	void y(int n)
	{
	}
};

struct consumer
{
	void a(int n)
	{
		std::cout << "consumer::a(" << n << ")" << std::endl;
	}

	void b(int n)
	{
		std::cout << "consumer::b(" << n << ")" << std::endl;
	}
};

/// \brief			Bridges two classes and their methods.
template<typename P, typename C, typename ... A>
class bridge
{
public:
	using signature_i = void(P:: *)(A ...);
	using signature_o = void(C:: *)(A ...);

public:
	bridge(signature_i i_ptr, C * consumer_ptr, signature_o o_ptr) :
		m_i_ptr(i_ptr),
		m_consumer_ptr(consumer_ptr),
		m_o_ptr(o_ptr)
	{
	}

	void invoke(A ... args)
	{
		(m_consumer_ptr->*m_o_ptr)(args ...);
	}

	bool operator ==(signature_i i_ptr)
	{
		return (m_i_ptr == i_ptr);
	}

	bool operator ==(signature_o o_ptr)
	{
		return (m_o_ptr == o_ptr);
	}

private:
	signature_i m_i_ptr;
	C * m_consumer_ptr;
	signature_o m_o_ptr;
};

int main()
{
	consumer c;
	bridge<producer, consumer, int> ba(&producer::x, &c, &consumer::b);

	if (ba == &producer::x)
	{
		if (ba == &consumer::b)
		{
			ba.invoke(7);
		}
	}

	return 0;
}

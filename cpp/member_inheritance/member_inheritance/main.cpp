#include <iostream>

class A
{
	public:
		static int m_static_member;
		int m_member;
};

class B : public A
{
	public:
		static int m_static_member;
		int m_member;

	public:
		void set_another_member()
		{
			A::m_member = 14;
			B::m_member = 3;		// explicit notation for: m_member = 3;
		}

		int get_base_another_member()
		{
			return A::m_member;
		}

		int get_another_member()
		{
			return m_member;	// non-explicit notation for: return B::m_member;
		}
};

int A::m_static_member = 0;
int B::m_static_member = 0;

int main(void)
{
	A a;
	a.m_static_member = 1;

	B b;
	b.m_static_member = 5;

	std::cout << a.m_static_member << std::endl;
	std::cout << b.m_static_member << std::endl;

	b.set_another_member();
	std::cout << b.get_another_member() << std::endl;
	std::cout << b.get_base_another_member() << std::endl;

	std::cout << b.A::m_member << std::endl;

	return 0;
}
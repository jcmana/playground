#include <iostream>

class base
{
	public:
		void Call()
		{
			Show();
		}

		void Show()
		{
			std::cout << "base::Show()" << std::endl;
		}
};

class derived : public base
{
	public:
		void Call()
		{
			__super::Call();
		}

		void Show()
		{
			std::cout << "derived::Show()" << std::endl;
		}
};


int main(void)
{
	derived D;
	D.Call();

	return 0;
}
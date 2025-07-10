#include <iostream>
#include <string>
#include <memory>

class IClass
{
	public:
		virtual void OnEvent() = 0;
};

class CClass : public IClass
{
	public:
		CClass()
		{

		}

		void Initialize()
		{
			std::cout << "CClass::Initialize()" << std::endl;
		}

		// IClass interface methods
		virtual void OnEvent() override
		{
			std::cout << "CClass::OnEvent()" << std::endl;
		}
};

int main()
{
	CClass c;
	c.Initialize();

	return 0;
}
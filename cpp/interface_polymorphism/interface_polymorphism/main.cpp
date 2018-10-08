#include <iostream>
#include <string>
#include <memory>

#include <windows.h>

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

void handler(LONG_PTR obj)
{
	IClass * iptr = (IClass *)(obj);
	iptr->OnEvent();

}

int main()
{
	CClass c;
	c.Initialize();

	LONG_PTR cptr = NULL;
	cptr = (LONG_PTR)(&c);

	handler(cptr);

	return 0;
}
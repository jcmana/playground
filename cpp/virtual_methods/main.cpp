#include <iostream>
#include <string>

namespace explicit_names {


class CAlpha
{
	public:
		void Call()
		{
			// Alpha
			this->CAlpha::MethodA();
		}

		virtual void MethodA()
		{
			std::cout << "   CAlpha::MethodA() @0x" << std::hex << (int)(this) << std::endl;
		}
};

class CBeta :
	virtual public CAlpha
{
	public:
		void Call()
		{
			// explicit Alpha
			this->CBeta::CAlpha::MethodA();

			// explicit Beta
			this->CBeta::MethodA();

			// implicit
			this->MethodA();
		}

		virtual void MethodA() override
		{
			std::cout << "   CBeta::MethodA() @0x" << std::hex << (int)(this) << std::endl;
		}
};


class CGama :
	virtual public CAlpha,
	virtual public CBeta
{
	public:
		void Call()
		{
			// explicit Alpha
			this->CGama::CAlpha::MethodA();
			this->CGama::CBeta::CAlpha::MethodA();

			// explicit Beta
			this->CGama::CBeta::MethodA();

			// explicit Gama
			this->CGama::MethodA();

			// implicit
			this->MethodA();
		}

		virtual void MethodA() override
		{
			std::cout << "   CGama::MethodA() @0x" << std::hex << (int)(this) << std::endl;
		}
};


} // namespace explicit_names


namespace vtable_inheritance {

class CAlpha
{
	public:
		virtual void A()
		{
			std::cout << "CAlpha::A()" << std::endl;
			B();
		}

		virtual void B()
		{
			std::cout << "CAlpha::B()" << std::endl;
		}
};

class CBeta : public CAlpha
{
public:
	virtual void A()
	{
		std::cout << "CBeta::A()" << std::endl;
		__super::A();
	}

	virtual void B()
	{
		std::cout << "CBeta::B()" << std::endl;
		__super::B();
	}
};

} // namespace vtable_inheritance

int main()
{
	// explicit_names
	{
		std::cout << "CAlpha::Call() : " << std::endl;
		explicit_names::CAlpha ca;
		ca.Call();

		std::cout << "CBeta::Call() : " << std::endl;
		explicit_names::CBeta cb;
		cb.Call();

		std::cout << "CGama::Call() : " << std::endl;
		explicit_names::CGama cc;
		cc.Call();
	}

	// vtable_inheritance
	{
		vtable_inheritance::CBeta beta;
		beta.A();
	}

	return 0;
}
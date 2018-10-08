#include <iostream>
#include <string>
#include <functional>

class CHandler
{
	public:
		CHandler(const std::string & name)
		{
			m_name = name;
		}

		void Handle()
		{
			std::cout << m_name << ": shit is handled, yo!" << std::endl;
		}

	private:
		std::string m_name;
};

void call(std::function<void()> & fn)
{
	fn();
}


int main()
{
	CHandler a("picus");
	CHandler b("kokot");

	std::function<void()> callback_ptr_a(std::bind(&CHandler::Handle, a));
	std::function<void()> callback_ptr_b(std::bind(&CHandler::Handle, b));

	call(callback_ptr_a);
	call(callback_ptr_b);

	return 0;
}

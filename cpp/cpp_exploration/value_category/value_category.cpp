#include <iostream>
#include <utility>
#include <string>

class s
{
public:
    std::string & text()
    {
        return m_text;
    }

private:
    std::string m_text = "hovno";
};


int main()
{
	// Binding to lvalue:
	{
		int n = 1;
		int & l = n;
		//int && r = n;		// error
	}

	// Binding to rvalue:
	{
		//int& l = 1;		// error
		int && r = 1;

		r = r + 3;
	}

    // Optional copy/move on return value:
    {
        s x;

        auto copy = x.text();
        auto move = std::move(x.text());
    }

	return 0;
}
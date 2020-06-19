#include <iostream>
#include <string>
#include <utility>

class s
{
public:
    s() :
        m_value("value")
    {
    }

    std::string value()
    {
        return std::move(m_value);
    }

private:
    std::string m_value;
};

void main()
{
    // Move local variable:
    std::string a("asdofmsadf");
    std::string b("vb2vb21vb21vb21");

    a = std::move(b);

    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";

    // Move from class function:
    s s_instance;

    const auto value = s_instance.value();

    std::cout << value << "\n";
}
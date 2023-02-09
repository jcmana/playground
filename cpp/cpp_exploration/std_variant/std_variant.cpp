#include <iostream>
#include <variant>
#include <string>

void f(std::string s)
{
    std::cout << "string" << std::endl;
}

void f(bool b)
{
    std::cout << "bool" << std::endl;
}

int main()
{
    std::variant<bool, std::string> bs;
    std::variant<std::string, bool> sb;
    std::variant<const char *, bool> cb;

    bs = {"abc"};
    sb = "abc";
    cb = "Abc";

    if (std::holds_alternative<std::string>(bs))
    {
        std::cout << "bs je cajk" << std::endl;
    }

    if (std::holds_alternative<std::string>(sb))
    {
        std::cout << "sb je cajk" << std::endl;
    }

    f("abc");
}
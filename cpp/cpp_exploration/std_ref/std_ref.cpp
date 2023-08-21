#include <iostream>
#include <utility>

struct s
{
    void method()
    {
        std::cout << "s::method()" << std::endl;
    }
};

void main()
{
    s a;
    auto r = std::ref(a);
    r(&s::method);
}

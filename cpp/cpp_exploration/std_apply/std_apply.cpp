#include <iostream>
#include <tuple>

void test(int i, const char * c, double d)
{
    std::cout << i << ", " << c << ", " << d << std::endl;
}

struct s
{
    void test(int i, const char * c, double d)
    {
        ::test(i, c, d);
    }
};

int main()
{
    const auto t = std::tuple {1, "asdfasdf", 0.5};

    std::apply(test, t);
    s ss;
    std::apply(&s::test, std::tuple_cat(std::tuple{&ss}, t));
}
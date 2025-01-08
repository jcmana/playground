#include <type_traits>
#include <string>

#include "ref.hpp"

void by_value(std::string s)
{
}

void by_pointer(std::string * ps)
{
}

void by_reference(ref<std::string> rs)
{
}

int main()
{
    std::string s = "7";
    std::string t = "42";

    ref a = &s;
    auto ra = a.get();

    const ref x = &s;
    auto rx = x.get();

    by_value(a);
    by_pointer(a);
    by_reference(a);
    
    by_reference(&s);

    return 0;
}
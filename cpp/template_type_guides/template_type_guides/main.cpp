#include <string>

#include "template.hpp"

deduction(const char *) -> deduction<std::string>;

void main()
{
    deduction a(7);
    deduction b("asdfasdf");
}
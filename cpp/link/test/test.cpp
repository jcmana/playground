#include <iostream>
#include <string>

#include "../link/link_element.hpp"
#include "../link/atomic_link_element.hpp"

int main()
{
    if (false)
    {
        link_element a;
        link_element b;
        link_element c;

        std::tie(a, b) = make_link();
        std::tie(c, b) = make_link();

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << "c.is_linked() = " << c.is_linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element x;
        std::tie(a, x) = make_link();

        link_element b;
        link_element y;
        std::tie(b, y) = make_link();

        swap(a, b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element b;
        std::tie(a, b) = make_link();

        swap(a, b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element x;
        std::tie(a, x) = make_link();

        link_element b;

        swap(a, b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element b;

        swap(a, b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        //link_element a_copy = a;      = deleted function
        //a_copy = a;                   = deleted function
    }

    if (true)
    {
        atomic_link_element a;
        atomic_link_element b;
        std::tie(a, b) = make_atomic_link();

        swap(a, b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << std::endl;

        atomic_link_element c = std::move(b);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << "c.is_linked() = " << c.is_linked() << std::endl;
        std::cout << std::endl;

        swap(b, a);

        std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
        std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
        std::cout << "c.is_linked() = " << c.is_linked() << std::endl;
        std::cout << std::endl;
    }

	return 0;
}
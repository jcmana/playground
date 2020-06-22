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

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << "c.linked() = " << c.linked() << std::endl;
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

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element b;
        std::tie(a, b) = make_link();

        swap(a, b);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element x;
        std::tie(a, x) = make_link();

        link_element b;

        swap(a, b);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        link_element b;

        swap(a, b);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << std::endl;
    }

    if (false)
    {
        link_element a;
        //link_element a_copy = a;      = deleted function
        //a_copy = a;                   = deleted function
    }

    if (false)
    {
        atomic_link_element a;
        atomic_link_element b;
        std::tie(a, b) = make_atomic_link();

        swap(a, b);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << std::endl;

        atomic_link_element c = std::move(b);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << "c.linked() = " << c.linked() << std::endl;
        std::cout << std::endl;
    }

    if (true) for (unsigned int n = 0; n < 10'000; ++n)
    {
        atomic_link_element a;
        atomic_link_element b;
        std::tie(a, b) = make_atomic_link();

        auto thread_proc = [](atomic_link_element l)
        {
            l.linked();
        };

        std::thread ta(thread_proc, std::move(a));
        swap(a, b);
        std::thread tb(thread_proc, std::move(b));

        tb.join();
        ta.join();
    }

	return 0;
}
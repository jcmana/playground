#include <iostream>
#include <future>
#include <string>

#include "../link/link_element.hpp"
#include "../link/atomic_link_element.hpp"
#include "../link/atomic_link_element_nosp.hpp"
#include "../link/atomic_link_element_asym.hpp"

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

        swap(b, a);

        std::cout << "a.linked() = " << a.linked() << std::endl;
        std::cout << "b.linked() = " << b.linked() << std::endl;
        std::cout << "c.linked() = " << c.linked() << std::endl;
        std::cout << std::endl;
    }

    if (false) 
    {
        for (unsigned int n = 0; n < 100'000; ++n)
        {
            atomic_link_element a;
            atomic_link_element b;
            std::tie(a, b) = make_atomic_link();

            auto thread_proc = [](atomic_link_element l)
            {
                l.linked();
            };

            auto aa = std::async(std::launch::async, thread_proc, std::move(a));
            swap(a, b);
            auto ab = std::async(std::launch::async, thread_proc, std::move(b));

            aa.wait();
            ab.wait();
        }
    }

    if (false)
    {
        std::future<void> f;

        {
            atomic_link_element a;
            atomic_link_element b;
            std::tie(a, b) = make_atomic_link();

            auto thread_proc = [](atomic_link_element l)
            {
                std::unique_lock<atomic_link_element> lock(l);
                std::cout << "l.linked() = " << l.linked() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "l.linked() = " << l.linked() << std::endl;
            };

            f = std::async(std::launch::async, thread_proc, std::move(a));
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        f.wait();

        std::cout << "done" << std::endl;
    }

    if (false)
    {
        atomic_link_element a;
        std::unique_lock<atomic_link_element> lock(a);
        //auto a_moved = std::move(a);        // deadlock = acquiring already locked mutex
    }

    if (false)
    {
        atomic_link_element_nosp a;
        atomic_link_element_nosp b;

        std::tie(a, b) = make_atomic_link_nosp();

        auto a_moved = std::move(a);
    }

    if (false)
    {
        asymetric_link_element::slave s;
        asymetric_link_element::master m;

        m.m_element_ptr = &s;
        s.m_element_ptr = &m;

        auto f = std::async([&]
        {
            std::unique_lock lock(s);
            std::cout << "slave locked" << std::endl;
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::unique_lock lock(m);
        std::cout << "master locked" << std::endl;
    }

    if (true)
    {
        for (auto n = 0; n != 1'000; ++n)
        {
            std::mutex m;
            std::mutex s;
        
            auto proc_m = [&]
            {
                m.lock();
                s.lock();
                //std::cout << "cricical section in master" << std::endl;
                s.unlock();
                m.unlock();
            };

            auto proc_s = [&]
            {
                s.lock();
                m.lock();
                //std::cout << "cricical section in slave" << std::endl;
                m.unlock();
                s.unlock();
            };

            auto tm = std::thread(proc_m);
            auto ts = std::thread(proc_s);

            ts.join();
            tm.join();
        }
    }

	return 0;
}

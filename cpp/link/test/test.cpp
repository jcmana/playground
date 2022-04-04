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
        atomic_link_element a;
        auto a_moved = std::move(a);
    }

    // link element without shared_ptr using double on each stack
    if (false)
    {
        nosp::atomic_link_element a;
        nosp::atomic_link_element b;

        nosp::atomic_link_element x;
        nosp::atomic_link_element y;

        std::tie(a, b) = nosp::make_atomic_link();
        std::tie(x, y) = nosp::make_atomic_link();

        swap(a, b);
        swap(y, x);

        swap(x, a);

        std::cout << "a linked = " << a.linked() << "\n";
        std::cout << "b linked = " << b.linked() << "\n";

        std::cout << "x linked = " << x.linked() << "\n";
        std::cout << "y linked = " << y.linked() << "\n";
    }

    if (true)
    {
        nosp::atomic_link_element a;

        auto proc = [&]
        {
            for (unsigned int n = 0; n < 5; ++n)
            {
                {
                    std::unique_lock l(a);
                    std::cout << "a linked = " << a.linked() << "\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        };
        auto t = std::async(proc);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            nosp::atomic_link_element b;

            std::tie(a, b) = nosp::make_atomic_link();

            {
                std::unique_lock l(b);
                std::cout << "b linked = " << b.linked() << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
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

    // asymetric locking:
    if (false)
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

    // double mutex dtor algorithm:
    if (false)
    {
        std::mutex lm;
        std::mutex rm;

        auto p = [&]
        {
            rm.lock();
            std::cout << "p locked\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            rm.unlock();

            rm.lock();
            std::cout << "p locked\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            rm.unlock();
        };
        auto t = std::thread(p);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        while (true)
        {
            lm.lock();

            if (rm.try_lock())
            {
                std::cout << "main locked\n";
                break;
            }
            else
            {
                lm.unlock();
            }
        }

        rm.unlock();
        lm.unlock();
        std::cout << "main unlocked\n";

        t.join();
    }

	return 0;
}

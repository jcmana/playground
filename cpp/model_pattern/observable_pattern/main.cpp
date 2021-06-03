#include <iostream>

#include <thread>
#include <future>
#include <functional>

#include "observable.hpp"
#include "observable_composite.hpp"
#include "observable_utility.hpp"

void cb(int value)
{
    std::cout << "modification = " << value << std::endl;
}

int main()
{
    // test interface #0
    if (false)
    {
        observable<int> o;
        auto ga = o.observe(cb);
        auto gb = o.observe(cb);
        auto gc = o.observe(cb);
        o.set(7);
    }

    // test interface #1
    if (false)
    {
        observable<int> o;
        auto g = o.observe(cb);

        o.modify([](int & v)
        {
            v = 7;
        });

        o.access([](int v)
        {
        });
    }

    // test interface #2
    if (false)
    {
        observable<int> o;
        auto g = o.observe(cb);

        {
            auto m = o.modify();
            static_cast<int &>(m) = 9;
        }

        {
            auto a = o.access();
            auto c = static_cast<const int &>(a);
            std::cout << c << std::endl;

            auto b = o.access();
            auto d = static_cast<const int &>(b);
            std::cout << d << std::endl;
        }
    }

    // test await
    if (false)
    {
        observable<int> o;

        auto f = std::async([&]
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "o = 3" << std::endl;
            o.set(3);

            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "o = 7" << std::endl;
            o.set(7);
        });

        await(o, {6, 7, 8, 9});
        std::cout << "modified" << std::endl;
    }

    // test join
    if (false)
    {
        observable<int> oa;
        observable<double> ob;

        auto f = [](int a, double b)
        {
            std::cout << "a = " << a << ", b = " << b << std::endl; 
        };

        decltype(join(f, oa, ob)) g = join(f, oa, ob);

        oa.set(15);
        ob.set(21.4);
        oa.set(-4);
    }

    if (false)
    {
        observable<int> oa;
        auto g = oa.observe(cb);

        auto oa_moved = std::move(oa);
        oa.set(7);
        oa_moved.set(7);

        oa_moved.trigger();
    }

    // observable composite
    if (true)
    {
        struct composite
        {
            int x;
            int y;
        };

        observable<int> oa;
        observable<int> ob;

        auto compose = [](int x, int y)
        {
            return composite{x, y};
        };
        observable_composite<composite, int, int> oc(compose, oa, ob);

        auto print = [](const composite & c)
        {
            std::cout << "x = " << c.x << ", y = " << c.y << std::endl;
        };
        auto g = oc.observe(print);

        // Test:
        oa.set(4);
        ob.set(7);
        ob.set(42);
        oa.set(0);
    }
}
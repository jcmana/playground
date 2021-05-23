#include <iostream>

#include <thread>
#include <future>
#include <functional>

#include "observable.hpp"
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

        decltype(join(oa, ob, f)) g = join(oa, ob, f);

        oa.set(15);
        ob.set(21.4);
        oa.set(-4);
    }

    if (true)
    {
        observable<int> oa;
        auto g = oa.observe(cb);

        auto oa_moved = std::move(oa);
        oa.set(7);
        oa_moved.set(7);
    }
}
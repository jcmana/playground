#include <iostream>

#include <thread>
#include <future>
#include <functional>
#include <memory>

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
    if (false)
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

    // observable composite (shared observable approach)
    if (false)
    {
        struct composite
        {
            std::shared_ptr<observable<int>> x;
            decltype(x)::element_type::guard_type gx;

            std::shared_ptr<observable<int>> y;
            decltype(y)::element_type::guard_type gy;
        };

        auto x = std::make_shared<observable<int>>(0);
        auto y = std::make_shared<observable<int>>(0);
        std::shared_ptr<observable<composite>> c;
        {
            c = std::make_shared<observable<composite>>();

            auto m = c->modify();
            m.get().x = x;
            m.get().y = y;
        }

        {
            auto observer_x = [&c](const int &)
            {
                c->trigger();
            };
            auto observer_y = [&c](const int &)
            {
                c->trigger();
            };

            auto m = c->modify();
            m.get().gx = m.get().x->observe(observer_x);
            m.get().gy = m.get().y->observe(observer_y);
        }

        auto observer = [](const composite & c)
        {
            std::cout << "x = " << c.x->get() << ", y = " << c.y->get() << std::endl;       // deadlock on get()
        };

        auto g = c->observe(observer);

        x->set(7);
        y->set(3);

        // THIS DOESN'T MAKE SENSE STRUCTURALLY
        // I don't wanna compose observables, only their value. That means struct composite is 
        // just weird and doesn't describe what I mean.
    }

    // observable composite (caching approach)
    if (false)
    {
        struct observer
        {
            observable<int>::guard_type guard_x;
            observable<int>::guard_type guard_y;

            int cached_x;
            int cached_y;

            std::function<void(int, int)> functor;
        };

        observable<int> x;
        observable<int> y;

        observer c;
        c.cached_x = x.get();
        c.cached_y = y.get();

        auto observer_c_x = [&c](int x)
        {
            c.cached_x = x;
            c.functor(c.cached_x, c.cached_y);
        };
        auto observer_c_y = [&c](int y)
        {
            c.cached_y = y;
            c.functor(c.cached_x, c.cached_y);
        };

        auto observer_c = [](int x, int y)
        {
            std::cout << "x = " << x << ", y = " << y << std::endl;
        };
        c.functor = observer_c;

        c.guard_x = x.observe(observer_c_x);
        c.guard_y = y.observe(observer_c_y);

        x.set(4);
        x.set(8);
        y.set(2);
        x.set(7);
        y.set(0);
    }

    // observer (mimicking signal/slot, but only for changes propagation)
    if (true)
    {
        using T = int;

        struct observer
        {
            observable<T>::guard_type guard;
            std::function<void(T)> functor;
        };

        observable<T> x;
        observable<T> y;

        observer ox;
        observer oy;
    }
}
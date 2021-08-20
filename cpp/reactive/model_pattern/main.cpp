#include <iostream>

#include <cassert>

#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <random>

#include "model.hpp"

void main()
{
    // modifier/accessor synchronization test:
    if (false)
    {
        model<int> m;

        const auto t_proc = [&]
        {
            std::cout << "t: accessing" << std::endl;
            auto mm = m.modifier();

            std::cout << "t: modifying" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "t: modifying done" << std::endl;
        };
        std::thread t;

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.accessor();

            t = std::thread(t_proc);

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        std::this_thread::yield();

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.accessor();

            std::cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: reading done" << std::endl;
        }

        t.join();
    }

    // model_guard test:
    if (false)
    {
        /*
        int n = 6;
        model<int> m(n);

        {
            auto mm = m.modifier_guard();
            mm.value() = 7;
            std::cout << mm.value() << std::endl; 
        }

        {
            auto ma = m.accessor_guard();
            std::cout << ma.value() << std::endl;
        }
        */
    }

    // wait test:
    if (false)
    {
        model<int> m;

        std::thread ta;
        std::thread tb;

        const auto t_proc = [&]
        {
            {
                std::cout << "t: accessing" << std::endl;
                auto mm = m.modifier();

                std::cout << "t: writing" << std::endl;
                mm.value() = 7;
                std::cout << "t: writing done" << std::endl;
            }
        };

        ta = std::thread(t_proc);

        {
            std::cout << "m: accessing" << std::endl;
            auto ma = m.wait();

            tb = std::thread(t_proc);

            std:: cout << "m: reading" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "m: value = " << ma.value() << std::endl;
            std::cout << "m: reading done" << std::endl;
        }

        ta.join();
        tb.join();
    }

    // model hierarchy:
    if (true)
    {
        struct a
        {
            model<int> n;
            model<double> u;
        };

        struct b
        {
            model<a> v;
            model<std::string> name;
        };

        model<b> k;
        
        k.modifier().value().v.modifier().value().n.modifier().value() = 4;
    }
}

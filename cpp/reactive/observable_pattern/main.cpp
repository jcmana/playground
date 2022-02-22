#include <iostream>

#include <thread>
#include <future>
#include <functional>
#include <memory>

#include "basic_observable.hpp"
#include "unique_observable.hpp"
#include "shared_observable.hpp"
#include "shared_ref_observable.hpp"

template<typename ... A>
using F = std::function<void(A ...)>;

template<typename ... A>
using Fptr = void(*)(A ...);

void cb(int value)
{
    std::cout << "modification = " << value << std::endl;
}

void cb()
{
}

template<typename T>
struct ref_specialization;

template<typename T>
struct ref_specialization
{
    T value;
};

template<typename T>
struct ref_specialization<T &>
{
    std::reference_wrapper<T> reference;
};

int main()
{
    // specialization for reference types
    if (false)
    {
        auto value = 0;
        auto sp_value = std::make_shared<int>(0);

        auto rsv = ref_specialization<int>{value};
        auto rsr = ref_specialization<int &>{*sp_value};
    }

    // basic observable (single type)
    if (false)
    {
        basic_observable<F, int> o{12};

        auto observer = [](int value)
        {
            std::cout << value << std::endl;
        };

        auto g = o.observe(observer);
        o.notify();
        o.get<0>() = 7;
        o.notify();
        o.get<0>() = 3;
        o.notify();
    }

    // basic observable (muliple types)
    if (false)
    {
        basic_observable<F, int, char> o;

        auto observer = [](int i, char c)
        {
            std::cout << i << ", " << c << std::endl;
        };

        auto g = o.observe(observer);
        o.notify();
        o.get<0>() = 7;
        o.get<1>() = 'f';
        o.notify();
        o.get<0>() = 3;
        o.get<1>() = 'k';
        o.notify();


        auto o_move = std::move(o);
        o_move = std::tuple{2, 'l'};
        o_move.notify();
        o.notify();

        {
            const auto & o_ref = o;
            auto c = o_ref.get<1>();
        }

        auto & [a, b] = o;
    }

    // basic observable (overloading)
    if (false)
    {
        // free function
        {
            basic_observable<Fptr, int> o;

            auto g = o.observe(cb);     // automatically takes the correct function
            o.notify();
            o.get<0>() = 4;
            o.notify();
        }

        // std::function
        {
            basic_observable<F, int> o;
            o.observe<void(*)(int)>(cb);
        }
    }

    // unique observable
    if (false)
    {
        unique_observable o{4, 'd'};

        auto observer = [](int i, char c)
        {
            std::cout << i << ", " << c << std::endl;
        };

        o.observe(observer);
        o.notify();
        o = std::tuple{12, 'x'};
        o.notify();

        auto o_moved = std::move(o);
        o.notify();
        o_moved.notify();

        o_moved.observe(observer);
        o_moved.notify();
        o.observe(observer);
        o.notify();
    }

    // unique and shared transactions
    if (true)
    {
        shared_observable o(0);

        auto observer = [](int i)
        {
            std::cout << i << std::endl;
        };
        o.observe(observer);

        auto proc = [o]
        {
            for (auto n = 0; n != 10; ++n)
            {
                unique_txn ug(o);
                ug = 0;
            }
        };
        std::thread t(std::move(proc));

        {
            unique_txn ug(o);
            ug = 4;
            ug = 15;
            ug = 1;
            // yields only a single notify
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            unique_txn ug(o);
            ug = 7;
        }

        {
            // access from two shared transactions
            shared_txn sg(o);
            std::cout << sg.get<0>() << std::endl;
            std::cout << shared_txn{o}.get<0>() << std::endl;
        }

        t.join();
    }

    // join shared observables
    if (false)
    {
        shared_observable<int> a;
        shared_observable<int> b;

        shared_observable<int, int> c = join(a, b);

        auto observer = [](int value_a, int value_b)
        {
            std::cout << value_a << ", " << value_b << std::endl;
        };
        c.observe(observer);

        unique_txn{a} = 7;
        unique_txn{b} = 4;
        unique_txn{c} = std::tuple{4, 8};
    }

    // compose shared observables
    if (false)
    {
        shared_observable<int> a;
        shared_observable<int> b;
        shared_observable<int> sum;

        auto observer_ab = [sum](int value_a, int value_b)
        {
            unique_txn{sum} = value_a + value_b;
        };
        join(observer_ab, a, b);

        auto observer_sum = [](int value_c)
        {
            std::cout << value_c << std::endl;
        };
        sum.observe(observer_sum);

        unique_txn{a} = 7;
        unique_txn{b} = 4;
    }

    // join shared observables (no composite)
    if (false)
    {
        shared_observable<int> a;
        shared_observable<int> b;

        auto observer = [](int value_a, int value_b)
        {
            std::cout << value_a << ", " << value_b << std::endl;
        };

        join(observer, a, b);

        unique_txn{b} = 4;
        unique_txn{a} = 7;
    }

    // await_if shared observable
    if (false)
    {
        shared_observable<int> a;

        auto proc = [a]
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = 4;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = 16;
        };
        std::thread t(proc);

        auto pred = [](int n)
        {
            return n == 16;
        };

        std::cout << "awaiting value" << std::endl;
        await_if(a, pred);
        std::cout << shared_txn{a}.get<0>() << std::endl;

        t.join();
    }

    // await_any shared observable
    if (false)
    {
        shared_observable<int, int> a;

        auto proc = [a]
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {4, 2};
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {16, 2};
        };
        std::thread t(proc);

        std::cout << "awaiting any change" << std::endl;
        await_any(a);
        std::cout << shared_txn{a}.get<0>() << ", " << shared_txn{a}.get<1>() << std::endl;

        t.join();
    }

    // await shared observable
    if (false)
    {
        shared_observable<int, int> a;

        auto proc = [a]
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {4, 2};
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {16, 8};
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {16, 2};
        };
        std::thread t(proc);

        std::cout << "awaiting value" << std::endl;
        await(a, 16, 2);
        std::cout << shared_txn{a}.get<0>() << ", " << shared_txn{a}.get<1>() << std::endl;

        t.join();
    }

    // shared observable without value (a shared signal)
    if (false)
    {
        shared_observable<void> so;

        auto observer = []
        {
            std::cout << "observer notified" << std::endl;
        };
        so.observe(observer);

        so.notify();

        auto proc = [so]
        {
            std::cout << "observer in thread notified" << std::endl;
        };
        std::thread t(proc);
        so.notify();
        t.join();
        so.notify();
    }

    // shared observarble of reference
    if (false)
    {
        shared_ref_observable<std::unique_ptr<int>> o;
        
        auto observer = [](const int & ref)
        {
            std::cout << ref << std::endl;
        };
        o.observe(observer);
        o.m_sp->notify();
        *o.m_sp->get<0>() = 5;
        o.m_sp->notify();
    }
}
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include <thread>
#include <future>
#include <functional>
#include <memory>

#include "observable.hpp"
#include "basic_obe_storage.hpp"

#include "breaking_circular_observers.h"
#include "circular_break_logic.hpp"

template<typename ... A>
using F = std::function<void(A ...)>;

template<typename ... A>
using Fptr = void(*)(const A & ...);

void cb(int value)
{
    std::cout << "modification = " << value << std::endl;
}

void cb(const int & value)
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

struct observer
{
    void cb(int value)
    {
        std::cout << "modification = " << value << std::endl;
    }
};

struct copy_check
{
    copy_check()
    {
    }

    copy_check(const copy_check & other)
    {
        std::cout << "copy constructor" << std::endl;
    }

    copy_check & operator  =(const copy_check & other)
    {
        std::cout << "copy assignment" << std::endl;
        return (*this);
    }
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
        basic_obe<int, F> o{12};

        auto observer = [](int value)
        {
            std::cout << value << std::endl;
        };

        auto g = o.observe(observer);
        o.notify();
        o.get() = 7;
        o.notify();
        o.get() = 3;
        o.notify();
    }

    // basic observable (overloading)
    if (false)
    {
        // free function
        {
            basic_obe<int, Fptr> o;

            auto g = o.observe(cb);     // automatically takes the correct function
            o.notify();
            o.get() = 4;
            o.notify();
        }

        // std::function
        {
            basic_obe<int, F> o;
            o.observe<void(*)(int)>(cb);
        }
    }

    // unique observable
    if (false)
    {
        unique_obe o{4};

        auto observer = [](int i)
        {
            std::cout << i << std::endl;
        };

        o.observe(observer);
        o.notify();
        o = 12;
        o.notify();

        auto o_moved = std::move(o);
        o.notify();
        o_moved.notify();

        o_moved.observe(observer);
        o_moved.notify();
        o.observe(observer);
        o.notify();
    }

    // shared observable on member function
    if (false)
    {
        observer o;
        
        shared_obe so{7};
        so.observe(&observer::cb, &o);

        unique_txn{so} = 12;
    }

    // unique and shared transactions
    if (false)
    {
        shared_obe o{0};

        auto observer = [](int i)
        {
            std::cout << i << std::endl;
        };
        o.observe(observer);

        auto proc = [o]() mutable
        {
            for (auto n = 0; n != 10; ++n)
            {
                unique_txn ug{o};
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
            std::cout << sg.get() << std::endl;
            std::cout << shared_txn{o}.get() << std::endl;
        }

        t.join();
    }

    // shared and unique transaction with non-blocking ctor
    if (false)
    {
        shared_obe<int> so;
        unique_txn a(so);
        unique_txn b(so, std::try_to_lock);

        if (b.owns_lock() == false)
        {
            std::cout << "b failed to achieve lock" << std::endl;
        }
    }

    // shared observable with ignored value
    if (false)
    {
        shared_obe<int> so;

        auto o = []
        {
            std::cout << "value changed" << std::endl;
        };
        so.observe(o);

        unique_txn{so} = 12;
    }

    // join shared observables
    if (false)
    {
        shared_obe<int> a;
        shared_obe<int> b;

        auto c = join(a, b);

        auto observer = [](std::tuple<int, int> value)
        {
            std::cout << std::get<0>(value) << ", " << std::get<1>(value)<< std::endl;
        };
        c.observe(observer);

        unique_txn{a} = 7;
        unique_txn{b} = 4;
        unique_txn{c} = std::tuple{4, 8};
    }

    // compose shared observables
    if (false)
    {
        shared_obe<int> a;
        shared_obe<int> b;
        shared_obe<int> sum;

        auto observer_ab = [sum](int value_a, int value_b) mutable
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

    // join 2 shared observables (no composite)
    if (false)
    {
        shared_obe<int> a;
        shared_obe<int> b;

        auto observer = [](int value_a, int value_b)
        {
            std::cout << value_a << ", " << value_b << std::endl;
        };
        join(observer, a, b);

        unique_txn{b} = 4;
        unique_txn{a} = 7;
    }

    // join 3 shared observables (no composite)
    if (false)
    {
        shared_obe<int> a;
        shared_obe<int> b;
        shared_obe<int> c;

        auto observer = [](int value_a, int value_b, int value_c)
        {
            std::cout << value_a << ", " << value_b << ", " << value_c << std::endl;
        };
        join(observer, a, b, c);

        unique_txn{b} = 4;
        unique_txn{a} = 7;
        unique_txn{c} = 1;
        unique_txn{a} = 8;
    }

    // await_if shared observable
    if (false)
    {
        shared_obe<int> a;

        auto proc = [a]() mutable
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
        auto txn = await_if(a, pred);
        std::cout << txn.get() << std::endl;

        t.join();
    }

    // await_any shared observable
    if (false)
    {
        shared_obe<std::tuple<int, int>> a;

        auto proc = [a]() mutable
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {4, 2};
            std::this_thread::sleep_for(std::chrono::seconds(1));
            unique_txn{a} = {16, 2};
        };
        std::thread t(proc);

        {
            std::cout << "awaiting any change" << std::endl;
            auto txn = await_any(a);
            std::cout << std::get<0>(txn.get()) << ", " << std::get<1>(txn.get()) << std::endl;
        }

        t.join();
    }

    // await shared observable
    if (false)
    {
        shared_obe<std::tuple<int, int>> a;

        auto proc = [a]() mutable
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
        await(a, std::tuple{16, 2});
        const auto v = shared_txn{a}.get();
        std::cout << std::get<0>(v) << ", " << std::get<1>(v) << std::endl;

        t.join();
    }

    // shared observable without value (a shared signal)
    if (false)
    {
        shared_obe<void> so;

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
        shared_obe<std::unique_ptr<int>> o;
        
        auto observer = [](const int & ref)
        {
            std::cout << ref << std::endl;
        };
        /*
        o.observe(observer);
        o->notify();
        o.get() = 5;
        o.notify();
        */
    }

    // forward value from one observable to another
    if (false)
    {
        shared_obe<int> soA;
        shared_obe<int> soB;

        auto observer_a = [](int value)
        {
            std::cout << "A changed = " << value << "\n";
        };

        auto observer_b = [](int value)
        {
            std::cout << "B changed = " << value << "\n";
        };

        soA.observe(observer_a);
        soB.observe(observer_b);

        unique_txn{soA} = 7;
        forward(soA, soB);
        unique_txn{soA} = 42;
        unique_txn{soB} = 1;
    }

    // benchmarking
    if (false)
    {
        constexpr unsigned int N = 100'000;

        std::vector<shared_obe<int>> so_list(N);
        constexpr auto s = sizeof(std::condition_variable);

        unsigned int sum = 0;
        auto observer = [&sum](const int & value)
        {
            sum += value;
        };

        // observe all
        for (auto & so : so_list)
        {
            so.observe(observer);
        }

        // update all
        for (unsigned int n = 0; n < N; ++n)
        {
            unique_txn{so_list[n]}.get() += n;
        }

        std::cout << "sum = " << sum << std::endl;
    }

    // deadlock detection
    if (false)
    {
        shared_obe<int> so(42);

        unique_txn a(so);
        unique_txn b(so);   // deadlocks
        shared_txn c(so);   // deadlocks
    }

    // basic observable storage
    if (false)
    {
        basic_obe_storage<int> a;
        auto a_get = a.get();

        basic_obe_storage<std::shared_ptr<int>> b;
        auto b_get = b.get();
    }

    // basic observable for shared pointers
    if (false)
    {
        basic_obe<std::shared_ptr<int>, F, basic_obe_storage> o;
        auto o_value = o.get();
        std::cout << o_value << std::endl;

        o.get() = 7;
        std::cout << int(o) << std::endl;

        auto observer = [](int value)
        {
            std::cout << value << std::endl;
        };
        auto g = o.observe(observer);
        o.get() = 2;
        o.notify();
    }

    // shared observable for smart pointers
    if (false)
    {
        shared_obe<std::shared_ptr<int>> so;

        auto observer = [](int value)
        {
            std::cout << value << std::endl;
        };
        so.observe(observer);

        unique_txn{so} = 7;
    }

    // basic observable with cv qualifiers
    if (false)
    {
        basic_obe<int, F> so;
        basic_obe<const int, F> so_const;
    }

    // shared observable with cv qualifiers
    if (false)
    {
        shared_obe<int> so;
        shared_obe<const int> so_const = so;
        //unique_txn tx{so_const};      // causes static_assert
        //unique_txn<const int> a;      // undefined ctor
        const auto v = shared_txn{so_const}.get();

        auto observer = [](int value)
        {
            std::cout << "value changed in shared_obe<const int> = " << value << std::endl;
        };
        so_const.observe(observer);

        unique_txn{so} = 7;
        unique_txn{so} = 2;
    }

    // copy checking during notification
    if (true)
    {
        shared_obe<copy_check> so;

        auto observer = [](const copy_check & cc)
        {
            // nop
        };
        so.observe(observer);

        unique_txn{so};
    }

    // upgrade from shared to unique txn
    if (true)
    {
        shared_obe<int> so;
        shared_txn txs(so);
        unique_txn<int> txu(std::move(txs));
    }

    // breaking ciruclar observers #1
    if (false)
    {
        //breaking_circular_observers();
        //breaking_circular_observers2();
    }

    // breaking ciruclar observers #2
    if (false)
    {
        shared_obe<int> source;
        shared_obe<int> target;

        auto source_observer = [](int value)
        {
            std::cout << "source value changed = " << value << std::endl;
        };
        source.observe(source_observer);

        auto target_observer = [](int value)
        {
            std::cout << "target value changed = " << value << std::endl;
        };
        target.observe(target_observer);

        circular_break_logic wbl(source, target);

        unique_txn{source} = 7;
        unique_txn{target} = 2;
    }

    _CrtDumpMemoryLeaks();
}
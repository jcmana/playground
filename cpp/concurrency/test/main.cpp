#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>

#include "../concurrency/barrier.hpp"
#include "../concurrency/latch.hpp"
#include "../concurrency/condition.hpp"

#include "../concurrency/atomic_queue.hpp"
#include "../concurrency/basic_executor.hpp"
#include "../concurrency/immediate_executor.hpp"
#include "../concurrency/executor.hpp"
//#include "../concurrency/executor_ordered_pool.hpp"

#include "../concurrency/memory.hpp"
#include "../concurrency/memory_guard.hpp"

#include "../concurrency/utility.hpp"

barrier barr_a;
barrier barr_b;

void print()
{
    std::cout << "print()" << std::endl;
}

void print_slow()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "print_slow()" << std::endl;
}

void print_parametric(const std::string & text)
{
    static std::mutex m;

    std::unique_lock<std::mutex> lock(m);
    std::cout << "print_parametric()" << text << std::endl;
}

int calculate(int n)
{
    return n * n;
}

void thread(const std::string & text)
{
    barr_a.arrive_and_wait();
    std::cout << "thread(): '" << text << "' start" << "\n";
    std::cout << "thread(): '" << text << "' stop" << "\n";
    barr_b.arrive_and_drop();
}

template<typename F>
void execute_task(F functor)
{
    functor();
}

template<typename T>
void execute_tasklist(T container)
{
    for (auto & task : container)
    {
        task();
    }
}

int main()
{
    if (false)
    {
        std::thread a(thread, "a");
        std::thread b(thread, "b");

        std::cout << "threads started" << "\n";
        barr_a.arrive_and_drop();

        barr_b.arrive_and_wait();
        std::cout << "threads stopped" << "\n";

        a.join();
        b.join();
    }

    if (false)
    {
        condition<int> ca;

        std::thread ta([&]
        {
            std::cout << "a:" << ca.wait() << std::endl;
            std::cout << "a:" << ca.wait() << std::endl;
            std::cout << "a:" << ca.wait() << std::endl;
        });
        std::thread tb([&]
        {
            std::cout << "b:" << ca.wait() << std::endl;
        });
        std::thread tc([&]
        {
            std::cout << "c:" << ca.wait() << std::endl;
        });

        std::cout << "modifying condition:" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ca = 4;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ca = 2;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ca = 17;

        ca = 7;
        ca = 7;

        ta.join();
        tb.join();
        tc.join();
        std::cout << "done" << std::endl;
    }
    
    /*
    if (false)
    {
        executor_ordered_pool<void> e(2);
        
        auto task = []
        {
            std::cout << "task in " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };

        std::vector<std::future<void>> futures;

        futures.emplace_back(e.post(task));
        futures.emplace_back(e.post(task));
        futures.emplace_back(e.post(task));
        futures.emplace_back(e.post(task));
        futures.emplace_back(e.post(task));
        futures.emplace_back(e.post(task));

        wait_all(futures);
    }

    if (false)
    {
        executor_queue<std::packaged_task<void()>> q;
        executor_thread<void> t(q);

        auto task = []
        {
            std::cout << "task in " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };

        q.push(std::packaged_task<void()>(task));
        q.push(std::packaged_task<void()>(task));
        q.push(std::packaged_task<void()>(task));
        q.push(std::packaged_task<void()>(task));
        q.push(std::packaged_task<void()>(task));
    }

    if (false)
    {
        class threadsafe_provider
        {
        public:
            auto size()
            {
                return m_value.lock();
            }

            void increment()
            {
                m_value.lock() = m_value.lock() + 1;
            }

            void reset()
            {
                m_value.lock() = 0;
            }

        private:
            memory<int> m_value;
        };

        threadsafe_provider a;
        a.reset();
        std::cout << a.size() << std::endl;

        auto thread_procedure = [&a]
        {
            for (unsigned int n = 0; n < 10; ++n)
            {
                a.increment();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };

        std::thread thread(thread_procedure);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << a.size() << std::endl;

        thread.join();

        auto mga = a.size();
        auto mgb = a.size();

        threadsafe_provider a_moved;
        a_moved = std::move(a);
    }

    if (true)
    {
        executor_ordered_pool<void> e(2);

        auto task = []
        {
            std::cout << "task in " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        };

        std::vector<std::future<void>> futures;

        futures.emplace_back(e.post(task));
        e.resize(3);
        futures.emplace_back(e.post(task));
    }
    */

    if (false)
    {
        auto e = basic_executor([]{});
        auto e_moved = std::move(e);
    }

    if (false)
    {   
        std::vector l = {print, print_slow, print};
        auto e = basic_executor(execute_tasklist<decltype(l)>, std::move(l));

        for (auto n : {1, 2, 3, 4})
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << n << std::endl;
        }
    }

    if (false)
    {   
        auto e = basic_executor(execute_task<void()>, print);

        for (auto n : {1, 2, 3, 4})
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << n << std::endl;
        }
    }

    if (false)
    {
        executor<int> e;
        e.post(calculate, 7);
        e.post(calculate, 12);
        e.post(calculate, 42);
    }

    if (true)
    {
        std::stringstream a;
        std::stringstream b;

        std::mutex m;

        auto proca = [&]
        {
            a << "a: thread id = " << std::this_thread::get_id() << "\n";
            std::unique_lock lock(m);
        };

        auto procb = [&]
        {
            b << "b: thread id = " << std::this_thread::get_id() << "\n";
            std::unique_lock lock(m);
        };

        {
            executor<void> ea;
            executor<void> eb;

            std::unique_lock lock(m);
            ea.post(proca);
            ea.post(proca);

            eb.post(procb);
            eb.post(procb);

            swap(ea, eb);
        }

        std::cout << a.str();
        std::cout << b.str();
    }
}

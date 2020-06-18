#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "../concurrency/barrier.hpp"
#include "../concurrency/latch.hpp"
#include "../concurrency/condition.hpp"

#include "../concurrency/executor_immediate.hpp"
#include "../concurrency/executor_ordered.hpp"
#include "../concurrency/executor_ordered_pool.hpp"
#include "../concurrency/executor_thread.hpp"

#include "../concurrency/memory.hpp"
#include "../concurrency/memory_guard.hpp"

#include "../concurrency/utility.hpp"

barrier barr_a;
barrier barr_b;

void print(const std::string & text)
{
    static std::mutex m;

    std::unique_lock<std::mutex> lock(m);
    std::cout << text << std::endl;
}

void thread(const std::string & text)
{
    barr_a.arrive_and_wait();
    std::cout << "thread(): '" << text << "' start" << "\n";
    std::cout << "thread(): '" << text << "' stop" << "\n";
    barr_b.arrive_and_drop();
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

    if (false)
    {
        executor_ordered<void> eo;

        auto task = []
        {
            std::cout << "task" << std::endl;
        };

        std::vector<std::future<void>> futures;

        futures.emplace_back(eo.post(task));
        futures.emplace_back(eo.post(task));
        futures.emplace_back(eo.post(task));

        wait_all(futures);

        {
            auto future1 = eo.post(task);
            auto future2 = eo.post(task);

            wait_all(future1, future2, std::future<int>());
        }

        std::cout << "fuck" << std::endl;
    }

    if (false)
    {
        {
            executor_ordered<void> eo;
            auto eo_move = std::move(eo);
        }

        {
            auto task = []
            {
                std::cout << "thread id = " << std::this_thread::get_id() << "\n";
            };

            executor_ordered<void> eo;
            executor_ordered<void> eo_move;

            eo.post(task);
            eo_move.post(task);

            eo_move = std::move(eo);

            eo.post(task);
            eo_move.post(task);
        }
    }

    if (false)
    {
        executor_immediate<int> ei;

        auto task = []
        {
            std::cout << "task" << std::endl;
            return 0;
        };

        ei.post(task);
    }

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
    }

    if (false)
    {
        executor_queue<std::packaged_task<void()>> q;
        executor_thread<void> t(q);
    }

    if (true)
    {
        class threadsafe_provider
        {
        public:
            auto size()
            {
                return m_value.get();
            }

            void increment()
            {
                m_value.get() = m_value.get() + 1;
            }

            void reset()
            {
                m_value.get() = 0;
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
}

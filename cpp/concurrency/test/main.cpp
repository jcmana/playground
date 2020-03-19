#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "../concurrency/barrier.hpp"
#include "../concurrency/latch.hpp"
#include "../concurrency/conversation.hpp"
#include "../concurrency/condition.hpp"

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

void conversionalist(int n)
{
    std::cout << "conversionalist(): '" << n << std::endl;
}

void conversionalist_b(int n)
{
    std::cout << "conversionalist_b(): " << n << std::endl;
}

void thread_conversationalist(const conversation & c)
{
    conversation local_c = c;
    local_c.enter(&conversionalist);


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
        // JMTODO: basic test
    }

    if (false)
    {
        conversation c;

        std::thread a(thread_conversationalist, c);
        std::thread b(thread_conversationalist, c);

        a.join();
        b.join();
    }

    if (true)
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
}

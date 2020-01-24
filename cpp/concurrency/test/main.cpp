#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "../concurrency/barrier.hpp"
#include "../concurrency/latch.hpp"
#include "../concurrency/conversation.hpp"

barrier barr_a;
barrier barr_b;

void thread(const std::string & text)
{
    barr_a.arrive_and_wait();
    std::cout << "thread(): '" << text << "' start" << "\n";
    std::cout << "thread(): '" << text << "' stop" << "\n";
    barr_b.arrive_and_wait();
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

        barr_a.arrive_and_drop();
        std::cout << "threads started" << "\n";

        barr_b.arrive_and_drop();
        std::cout << "threads stopped" << "\n";

        a.join();
        b.join();
    }

    if (false)
    {
        // JMTODO: basic test
    }

    if (true)
    {
        conversation c;

        std::thread a(thread_conversationalist, c);
        std::thread b(thread_conversationalist, c);

        a.join();
        b.join();
    }
}

#include <iostream>

#include <thread>
#include <mutex>

#include "../memory_guard/memory.hpp"

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

void main()
{
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

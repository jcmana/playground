#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace stlab;

void main()
{
    auto f = [](int x)
    {
        return 42 + x;
    };

    auto p = package<void()>(default_executor, f);
}
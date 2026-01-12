#include <iostream>

#include "tracing_exception.hpp"

void throwing_function()
{
    throw tracing_exception("throwing_function() failed");
}

struct test
{
    void execute()
    {
        throwing_function();
    }
};

int main()
{
    
    try
    {
        test t;
        t.execute();
    }
    catch (const tracing_exception & ex)
    {
        std::cout << "main() failed with exception: '" << ex.what() << "' at:" << std::endl;
        std::cout << ex.trace() << std::endl;
    }

    return 0;
}
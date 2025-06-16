#include <iostream>

#include <exception>
#include <stacktrace>

class tracing_exception : public std::exception
{
public:
    tracing_exception(std::string_view what) :
        std::exception(what.data())
    {
        m_stacktrace = std::stacktrace::current();
    }

    tracing_exception() :
        std::exception()
    {
        m_stacktrace = std::stacktrace::current();
    }

    const std::stacktrace & trace() const
    {
        return m_stacktrace;
    }

private:
    std::stacktrace m_stacktrace;
};

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
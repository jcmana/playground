#include <iostream>

int good()
{
    return 7;
}

int bad()
{
    return -2;
}

int main()
{
    // Classic return value error:
    if (auto e = good(); e > 0)
    {
        std::cout << "good() failed: " << e << std::endl;
    }

    if (auto e = bad(); e > 0)
    {
        std::cout << "bad() failed: " << e << std::endl;
    }

    // Exceptional error:
    try
    {
        throw good();
        throw bad();
        throw bad();
    }
    catch (int e)
    {
        std::cout << "caught: " << e << std::endl;
    }

    // JMTODO: how to cross between exceptions and return codes on adjacent program layers?
    // - inner layer returns error
    // - middle layer throws it
    // - outter layer expects return code again

    return 0;
}
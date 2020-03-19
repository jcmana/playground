#include <cstdarg>

#include <iostream>

void a(int n, ...);
void b(int n, va_list args);

void a(int n, ...)
{
    va_list args;

    va_start(args, n);
    b(n, args);
    va_end(args);
}

void b(int n, va_list args)
{
    for (int counter = 0; counter < n; ++counter)
    {
        int arg = va_arg(args, decltype(arg));
        std::cout << arg << " ";
    }
}

void main()
{
    a(4, 12, 13, 14, 15);
}
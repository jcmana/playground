#include <iostream>

struct stack
{
    int result = 0;
    int n = 0;
    int k = 4;
};

enum label
{
    begin,
    a,
    b,
    end,
};

struct frame
{
    stack scope;
    label returnpoint;
};

auto coroutine(stack scope = {}, label entrypoint = {}) 
{ 
    switch (entrypoint)
    {
        case begin:
            scope.n = 0;
            scope.k = 0;
            entrypoint = a;
        case a:
            scope.n++;
            scope.k++;
            entrypoint = b; 
        break;
        case b:
            scope.n = 17;
            scope.k = 3;
            entrypoint = end; 
        break;
        case end:
        break;
    }

    return frame{scope, entrypoint};
}

void main()
{
    auto [scope1, address1] = coroutine();
    std::cout << scope1.n << ", " << scope1.k << ", " << address1 << std::endl;
    auto [scope2, address2] = coroutine(scope1, address1);
    std::cout << scope2.n << ", " << scope2.k << ", " << address2 << std::endl;
    auto [scope3, address3] = coroutine(scope2, address2);
    std::cout << scope3.n << ", " << scope3.k << ", " << address3 << std::endl;
}

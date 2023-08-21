#include <iostream>
#include <string>

#include "../error/error_store.hpp"

// {8D7B7918-E178-4518-995A-DD617C0445D6}
static constexpr error OK = 
{0x8d7b7918, 0xe178, 0x4518, {0x99, 0x5a, 0xdd, 0x61, 0x7c, 0x4, 0x45, 0xd6}};

// {979D6F7C-C619-4BC9-9402-7834C3DC7AD6}
static constexpr error UNKNOWN = 
{0x979d6f7c, 0xc619, 0x4bc9, {0x94, 0x2, 0x78, 0x34, 0xc3, 0xdc, 0x7a, 0xd6}};

int main()
{
    error_store<const char *> s("Unknown error");
    s.set(OK, "OK");

    std::cout << s.get(OK) << std::endl;
}
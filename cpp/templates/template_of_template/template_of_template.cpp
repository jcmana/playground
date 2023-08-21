#include <vector>
#include <type_traits>

template<template<typename> typename T>
struct tot
{
};

int main()
{
    tot<std::vector> t;
    (void)(t);
}
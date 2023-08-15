#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <tuple>

struct part
{
    int value;
};

struct whole
{
    unsigned int id;
    part a;
    part b;
};

template <typename Tuple, std::size_t ... Is>
auto pop_front_impl(const Tuple & tuple, std::index_sequence<Is ...>)
{
    return std::make_tuple(std::get<1 + Is>(tuple) ...);
}

template <typename Tuple>
auto pop_front(const Tuple & tuple)
{
    return pop_front_impl(tuple, std::make_index_sequence<std::tuple_size<Tuple>::value - 1>());
}

/*
template<typename T, typename P>
auto get(T & object, const P & path)
{
    if constexpr (std::tuple_size(path) == 1)
    {
        auto ptr = std::get<0>(path);
        return (object.*ptr);
    }

    return get(object, std::make_tuple());
}
*/

template<typename T, typename ... A>
constexpr auto get(T & object, A ... args)
{
    return (object .* ... .* args);
}

int main()
{
    // #1: Simple inefficient JSON-like type-unsafe approach
    std::vector<std::variant<std::string, std::size_t>> json_path;

    // #2: Typesafe C++ variadric hell
    std::tuple member_pointer_path = 
    {
        //&whole::id
        &whole::a,
        &part::value,
    };

    whole x;
    x.id = 7;
    x.a.value = 12;

    //std::cout << get(x, member_pointer_path) << std::endl;

    // #3: Typesafe direct variadric template
    std::cout << get(x, &whole::a, &part::value) << std::endl;
}
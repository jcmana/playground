#include <tuple>
#include <array>
#include <optional>

template<typename T, typename A, typename B>
struct mapped_type
{
    using F = std::decay_t<T>;
    static_assert(std::is_same_v<F, A> || std::is_same_v<F, B>, "From must be either A or B");
    using type = std::conditional_t<std::is_same_v<F, A>, B, A>;
};

template<typename T, typename A, typename B>
using mapped_type_t = typename mapped_type<T, A, B>::type;


/*
template<typename T, typename A, typename B>
struct other_type;

template<typename T>
struct other_type<T, T, T>
{
    static_assert("A, B and T are the same");
};

template<typename T, typename A>
struct other_type<T, A, T>
{
    using type = A;
};

template<typename T, typename B>
struct other_type<T, T, B>
{
    using type = B;
};

template<typename T, typename A>
struct other_type<T, A, A>
{
    static_assert("A and B are the same");
};

template<typename T, typename A, typename B>
using other_type_t = typename other_type<T, A, B>::type;
*/

void undefined_value_mapping();


template<typename A, typename B, std::size_t N>
constexpr
std::size_t
find(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    for (std::size_t index = 0; index != N; index++)
    {
        if (std::get<A>(mapping[index]) == value)
        {
            return index;
        }
    }

    return N;
}

template<typename A, typename B, std::size_t N>
constexpr
std::size_t
find(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    for (std::size_t index = 0; index != N; index++)
    {
        if (std::get<B>(mapping[index]) == value)
        {
            return index;
        }
    }

    return N;
}

template<typename From, typename A, typename B, std::size_t N>
consteval
mapped_type_t<From, A, B>
map(const From & value, std::tuple<A, B> const (& mapping)[N])
{
    using FromDecayed = std::decay_t<From>;
    using To = mapped_type_t<From, A, B>;

    constexpr bool forward = std::is_same_v<FromDecayed, A> && std::is_same_v<To, B>;
    constexpr bool backward = std::is_same_v<FromDecayed, B> && std::is_same_v<To, A>;

    static_assert(forward || backward, "Invalid conversion direction for this mapping");

    for (auto const & tuple : mapping)
    {
        if constexpr (forward)
        {
            if (std::get<0>(tuple) == value)
            {
                return std::get<1>(tuple);
            }
        }
        else
        {
            if (std::get<1>(tuple) == value)
            {
                return std::get<0>(tuple);
            }
        }
    }

    throw "value mapping doesn't exist";
}

template<typename A, typename B, std::size_t N>
consteval
B
map2(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    /*
    for (auto const & tuple : mapping)
    {
        if (std::get<A>(tuple) == value)
        {
            return 
            {
                std::get<B>(tuple)
            };
        }
    }
    
    undefined_value_mapping();
    //throw "value mapping doesn't exist";
    */

    const auto i = find(value, mapping);

    if (i == N)
    {
        undefined_value_mapping();
    }

    return std::get<B>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
consteval
A
map2(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        undefined_value_mapping();
    }

    return std::get<A>(mapping[i]);
}


template<typename A, typename B, std::size_t N>
std::optional<A>
map3(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    /*
    for (auto const & tuple : mapping)
    {
        if (std::get<B>(tuple) == value)
        {
            return
            {
                std::get<A>(tuple)
            };
        }
    }
    
    return std::nullopt;
    */

    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<A>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
std::optional<B>
map3(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    /*
    for (auto const & tuple : mapping)
    {
        if (std::get<B>(tuple) == value)
        {
            return
            {
                std::get<A>(tuple)
            };
        }
    }

    return std::nullopt;
    */

    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<B>(mapping[i]);
}


template<typename A, typename B, std::size_t N>
using bidirectional_map = std::tuple<int, double>[N];

//static constexpr std::tuple<int, double> MAPPING[] = 
static constexpr bidirectional_map<int, double, 2> MAPPING = 
{
    {4, 12.3},
    {8, 19.8},
};

int main()
{
    // find():
    {
        constexpr auto i = find(5, MAPPING);
    }

    // map():
    {
        //const auto a = map(6, MAPPING);
        const auto b = map(8, MAPPING);
    
        const auto c = map(12.3, MAPPING);
        //const auto d = map(12.8, MAPPING);
    }

    // map2():
    {
        const auto a = map2(8, MAPPING);
        //const auto b = map2(6, MAPPING);

        const auto c = map2(12.3, MAPPING);
        //const auto d = map2(12.65, MAPPING);
    }

    // runtime:
    {
        const auto a = map3(6.2, MAPPING);
        const auto b = map3(12.3, MAPPING);
        const auto c = map3(4, MAPPING);
    }

    /*
    {
        using t = other_type_t<double, double, int>;

        using u = other_type_t<double, double, double>;
        //using v = other_type_t<double, int, char>;
    }
    */
}

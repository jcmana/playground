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

void undefined_value_mapping();

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
}

template<typename A, typename B, std::size_t N>
consteval
A
map2(const B & value, std::tuple<A, B> const (&mapping)[N])
{
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

    //throw "value mapping doesn't exist";
    undefined_value_mapping();
}


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
std::optional<A>
map3(const B & value, std::tuple<A, B> const (&mapping)[N])
{
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
    }
}

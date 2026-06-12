#include <tuple>
#include <array>
#include <optional>

template<typename A, typename B>
using bidirectional_map = std::tuple<A, B>[];

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

template<typename A, typename B, std::size_t N>
consteval
const B &
cmap(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        undefined_value_mapping();
    }

    return std::get<B>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
consteval
const A &
cmap(const B & value, std::tuple<A, B> const (&mapping)[N])
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
map(const B & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<A>(mapping[i]);
}

template<typename A, typename B, std::size_t N>
std::optional<B>
map(const A & value, std::tuple<A, B> const (&mapping)[N])
{
    const auto i = find(value, mapping);

    if (i == N)
    {
        return std::nullopt;
    }

    return std::get<B>(mapping[i]);
}


static constexpr bidirectional_map<int, double> MAPPING = 
{
    {4, 12.3},
    {8, 19.8},
};

static constexpr bidirectional_map<int, const char *> MAPPING_INT_STR =
{
    {4, "asdfsdf"},
    {8, "ab"},
    {0, "abasdfsaddsagsdf"},
};

int main()
{
    // find():
    {
        constexpr auto i = find(5, MAPPING);
    }

    // cmap():
    {
        //const auto a = cmap(6, MAPPING);
        const auto b = cmap(8, MAPPING);
    
        const auto c = cmap(12.3, MAPPING);
        //const auto d = cmap(12.8, MAPPING);

        //const auto e = cmap("ab", MAPPING_INT_STR);
        const auto f = cmap(8, MAPPING_INT_STR);
    }

    // runtime:
    {
        const auto a = map(6.2, MAPPING);
        const auto b = map(12.3, MAPPING);
        const auto c = map(4, MAPPING);

        //const auto d = map("ax", MAPPING_INT_STR);
    }
}

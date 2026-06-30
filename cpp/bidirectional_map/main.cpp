#include "bidirectioncal_map.hpp"

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

struct usertype
{
    int n = 0;

    constexpr bool operator ==(const usertype & other) const
    {
        return (n == other.n);
    }
};
static constexpr bidirectional_map<bool, usertype> MAPPING_USERTYPE =
{
    {true, usertype{7}},
    {false, usertype{12}},
};

int main()
{
    // find():
    {
        const auto i = find(5, MAPPING);
        const auto j = find(usertype{7}, MAPPING_USERTYPE);
    }

    // cmap():
    {
        //const auto a = cmap(6, MAPPING);
        const auto b = cmap(8, MAPPING);
    
        const auto c = cmap(12.3, MAPPING);
        //const auto d = cmap(12.8, MAPPING);

        const auto e = cmap<int, const char *>("ab", MAPPING_INT_STR);
        //const auto f = cmap("ab", MAPPING_INT_STR);
        const auto g = cmap(8, MAPPING_INT_STR);

        constexpr auto h = cmap<bool, usertype>(false, MAPPING_USERTYPE);
        constexpr auto i = find(false, MAPPING_USERTYPE);
        constexpr auto j = cmap<bool, usertype>(usertype{7}, MAPPING_USERTYPE);
    }

    // map():
    {
        const auto a = map(6.2, MAPPING);
        const auto b = map(12.3, MAPPING);
        const auto c = map(4, MAPPING);

        const auto d = map<int, const char *>("ax", MAPPING_INT_STR);
    }
}

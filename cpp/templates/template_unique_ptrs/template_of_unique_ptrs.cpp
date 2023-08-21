#include <memory>
#include <string>
#include <type_traits>

template<typename T, template <typename ...> typename U>
struct is_instance : std::false_type 
{
};

template<typename T, template <typename ...> typename U>
struct is_instance<U<T>, U> : std::true_type 
{
};

template<typename T, typename = void>
class test;

template<typename T>
struct test<T, std::enable_if_t<is_instance<T, std::unique_ptr>::value == false>>
{
    constexpr static int v = 0;
};

template<typename T>
struct test<std::unique_ptr<T>, std::enable_if_t<std::is_same_v<T, int>>>
{
    constexpr static int v = 1;
};

int main()
{
    constexpr auto a = test<int>::v;
    constexpr auto b = test<std::unique_ptr<int>>::v;
}
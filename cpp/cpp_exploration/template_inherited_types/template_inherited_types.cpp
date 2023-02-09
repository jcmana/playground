#include <iterator>

template<typename T>
struct base
{
    using value_type = T;
};

template<typename T>
struct spec : base<T>
{
    typename base<T>::value_type method() const
    {
        return typename base<T>::value_type();
    }
};

struct it : std::iterator<std::input_iterator_tag, int>
{
};

int main()
{
    spec<int> x;
    x.method();
}
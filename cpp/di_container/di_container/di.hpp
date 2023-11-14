#pragma once

template<typename T, unsigned int N>
class di_overload
{
};

template<typename ... A>
class di
{
public:
    di(A ... dependencies) :
        m_dependencies(std::forward<A>(dependencies) ...)
    {
    }

    template<typename T>
    T & get() const
    {
        return std::get<T>(m_dependencies);
    }

private:
    std::tuple<A ...> m_dependencies;
};
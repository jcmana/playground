#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <tuple>

using ptr_t = void *;

template<typename F>
constexpr auto pointer_to_member_id(F member_ptr)
{
    return static_cast<ptr_t >(*reinterpret_cast<const ptr_t  *>(&member_ptr));
}

template<typename F>
constexpr auto member_id_to_pointer(ptr_t member_id)
{
    return *reinterpret_cast<F *>(&member_id);
}

/// \brief      Listens to callbacks `C` and relays them to `T`.
/// \param      C       Callback interface type.
/// \param      T       Relay interface type.
template<typename C, typename T>
class callback_listener
{
public:
    template<typename F, typename G>
    void bind(F && source_method, G && target_method, T & that)
    {
        const auto source_id = pointer_to_member_id(source_method);
        const auto target_id = pointer_to_member_id(target_method);

        m_binding[source_id] = std::make_tuple(target_id, reinterpret_cast<ptr_t >(&that));
    }

    template<typename F, typename ... A>
    void invoke(F && method, A ... arguments)
    {
        using signature = void(T:: *)(A ...);

        auto source_id = pointer_to_member_id(method);
        auto binding = m_binding[source_id];
        auto target_id = std::get<0>(binding);
        auto target_that = std::get<1>(binding);

        auto target_ptr = reinterpret_cast<T *>(target_that);
        auto target_method_ptr = member_id_to_pointer<signature>(target_id);

        (target_ptr->*target_method_ptr)();
    }

private:
    std::map<ptr_t , std::tuple<ptr_t, ptr_t>> m_binding;
};

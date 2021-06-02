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

/// \brief      Listens to `callback` meant for `S` and relays them to `T`.
/// \param      S       Source interface type.
/// \param      T       Target interface type.
template<typename S, typename T>
class callback_ref_listener
{
public:
    template<typename ... A>
    void bind(void(S:: * source_method_ptr)(A ...), void(T:: * target_method_ptr)(A ...), T & that)
    {
        const auto source_id = pointer_to_member_id(source_method_ptr);
        const auto target_id = pointer_to_member_id(target_method_ptr);

        m_binding[source_id] = std::make_tuple(target_id, reinterpret_cast<ptr_t >(&that));
    }

    template<typename ... A>
    void operator ()(void(S:: * method_ptr)(A ...), A ... args)
    {
        const auto source_id = pointer_to_member_id(method_ptr);
        const auto it = m_binding.find(source_id);

        if (it == m_binding.end())
        {
            return;
        }

        const auto binding = it->second;
        const auto target_id = std::get<0>(binding);
        const auto target_that = std::get<1>(binding);

        const auto target_ptr = reinterpret_cast<T *>(target_that);
        const auto target_method_ptr = member_id_to_pointer<void(T:: *)(A ...)>(target_id);

        (*target_ptr.*target_method_ptr)(std::forward<A>(args) ...);
    }

private:
    std::map<ptr_t , std::tuple<ptr_t, ptr_t>> m_binding;
};

#pragma once

#include <cassert>
#include <type_traits>

/// \brief      Ensures thread-safe unique modification of `model`.
///
/// Modifier has to be held through the whole process of model value modification. After
/// releasing the modifier, changes are announced to waiting and observing threads.
template<typename M, typename T>
class model_guard
{
public:
    template<typename TT>
    friend class model;

public:
    /// \brief      Copy constructor, deleted.
    model_guard(const model_guard & other) = delete;

    /// \brief      Move constructor, overtakes critical section from `other`.
    model_guard(model_guard && other) = default;

    /// \brief      Destructor, releases critical section from `model`.
    ~model_guard();

    /// \brief      Modifiable model value.
    typename std::enable_if_t<std::is_const<M>::value != true, T> &
    value();

    /*
    typename std::enable_if_t<std::is_const<M>::value == true, const T> &
    value() const
    {
        return m_model_ref.m_value;
    }
    */

    void release() const
    {
        m_model_ref.m_mutex.unlock();

        if (std::enable_if_t<std::is_const<M>::value)
        {
            m_model_ref.trigger();
        }
    }

private:
    /// \brief      Constructor, extends critical section from `model_ref`.
    explicit model_guard(M & model_ref);

private:
    /// \brief      Model reference.
    M & m_model_ref;
};

#pragma region model_guard implementation:

template<typename M, typename T>
model_guard<M, T>::~model_guard()
{
    // Unlock mutex before triggering the notifications, otherwise we would
    // cause a deadlock, because mutex is accessed during notification handling
   // m_model_ref.m_mutex.unlock();

    //m_model_ref.trigger();
    release();
}

template<typename M, typename T>
typename std::enable_if_t<std::is_const<M>::value != true, T> &
model_guard<M, T>::value()
{
    return m_model_ref.m_value;
}

template<typename M, typename T>
model_guard<M, T>::model_guard(M & model_ref) :
    m_model_ref(model_ref)
{
    // m_model_ref.m_mutex needs to be already locked here by model<T>, therefore
    // model_guard now extends the duration of critical section

    assert(m_model_ref.m_mutex.try_lock() == false);
}

#pragma endregion

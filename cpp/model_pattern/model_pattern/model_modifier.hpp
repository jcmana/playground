#pragma once

#include <cassert>

/// \brief      Ensures thread-safe unique modification of `model`.
///
/// Modifier has to be held through the whole process of model value modification. After
/// releasing the modifier, changes are announced to waiting and observing threads.
template<typename T>
class model_modifier
{
public:
    template<typename TT>
    friend class model;

public:
    /// \brief      Copy constructor, deleted.
    model_modifier(const model_modifier & other) = delete;

    /// \brief      Move constructor, overtakes critical section from `other`.
    model_modifier(model_modifier && other) = default;

    /// \brief      Destructor, releases critical section from `model`.
    ~model_modifier();

    /// \brief      Modifiable model value.
    T & value();

private:
    /// \brief      Constructor, extends critical section from `model_ref`.
    explicit model_modifier(model<T> & model_ref);

private:
    /// \brief      Writeable model reference.
    model<T> & m_model_ref;
};

#pragma region model_modifier implementation:

template<typename T>
model_modifier<T>::~model_modifier()
{
    // Unlock mutex before triggering the notifications, otherwise we would
    // cause a deadlock, because mutex is accessed during notification handling
    m_model_ref.m_mutex.unlock();

    m_model_ref.trigger();
}

template<typename T>
T &
model_modifier<T>::value()
{
    return m_model_ref.m_value;
}

template<typename T>
model_modifier<T>::model_modifier(model<T> & model_ref) :
    m_model_ref(model_ref)
{
    // m_model_ref.m_mutex needs to be already locked here by model<T>, therefore
    // model_modifier now extends the duration of critical section

    assert(m_model_ref.m_mutex.try_lock() == false);
}

#pragma endregion

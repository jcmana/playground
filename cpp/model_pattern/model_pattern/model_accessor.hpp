#pragma once

#include <cassert>

/// \brief      Ensures thread-safe unique access to `model`.
///
/// `model` value is constant for the lifetime of this object, meaning nobody
/// can read or modify the `model` value.
template<typename T>
class model_accessor
{
public:
    template<typename TT>
    friend class model;

public:
    /// \brief      Copy constructor, deleted.
    model_accessor(const model_accessor & other) = delete;

    /// \brief      Move constructor, overtakes critical section from `other`.
    model_accessor(model_accessor && other) = default;

    /// \brief      Destructor, releases critical section from `model`.
    ~model_accessor();

    /// \brief      Readable model value reference.
    const T & value() const;

private:
    /// \brief      Constructor, extends critical section from `model_ref`.
    explicit model_accessor(const model<T> & model_ref);

private:
    /// \brief      Readable model reference.
    const model<T> & m_model_ref;
};

#pragma region model_modifier implementation:

template<typename T>
model_accessor<T>::~model_accessor()
{
    m_model_ref.m_mutex.unlock();
}

template<typename T>
const T &
model_accessor<T>::value() const
{
    return m_model_ref.m_value;
}

template<typename T>
model_accessor<T>::model_accessor(const model<T> & model_ref) :
    m_model_ref(model_ref)
{
    // m_model_ref.m_mutex needs to be already locked here by model<T>, therefore
    // model_accessor now extends the duration of critical section

    assert(m_model_ref.m_mutex.try_lock() == false);
}

#pragma endregion

#pragma once

/// \brief      Ensures thread-safe unique access of `model`.
template<typename T>
class model_accessor
{
public:
    template<typename TT>
    friend class model;

public:
    model_accessor(const model_accessor & other) = delete;

    model_accessor(model_accessor && other) = default;

    ~model_accessor()
    {
        m_model_ref.m_mutex.unlock();
    }

    /// \brief      Readable model value reference.
    const T & value() const
    {
        return m_model_ref.m_value;
    }

private:
    /// \brief      Constructor, extends the lock from `model<T>`.
    explicit model_accessor(const model<T> & model_ref) :
        m_model_ref(model_ref)
    {
        // m_model_ref.m_lock needs to be already locked here by model<T>, therefore
        // model_accessor now extends the duration of critical section
    }

private:
    /// \brief      Accessed model reference.
    const model<T> & m_model_ref;
};
#pragma once

template<typename T>
class model_accessor
{
public:
    template<typename TT>
    friend class model;

public:
    const T & value() const
    {
        return m_model_ref.m_value;
    }

private:
    model_accessor(model<T> & model_ref) :
        m_lock(model_ref.m_mutex),
        m_model_ref(model_ref)
    {
    }

private:
    mutable std::unique_lock<std::mutex> m_lock;

    /// \brief      Accessed model reference.
    model<T> & m_model_ref;
};
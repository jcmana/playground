#pragma once

template<typename T>
class model_modifier
{
public:
    template<typename TT>
    friend class model;

public:
    model_modifier(model_modifier && other) = default;

    ~model_modifier()
    {
        m_model_ref.trigger();
    }

    T & value()
    {
        return m_model_ref.m_value;
    }

private:
    model_modifier(model<T> & model_ref) :
        m_lock(model_ref.m_mutex),
        m_model_ref(model_ref)
    {
    }

private:
    mutable std::unique_lock<std::mutex> m_lock;

    /// \brief      Modified model reference.
    model<T> & m_model_ref;
};

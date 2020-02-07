#pragma once

/// \brief      Ensures thread-safe unique modification of `model`.
///
/// Modifier has to be held through the whole process of model value modification. After
/// releasing the modifier, changes are announec to waiting and observing threads. 
template<typename T>
class model_modifier
{
public:
    template<typename TT>
    friend class model;

public:
    model_modifier(const model_modifier & other) = delete;
    model_modifier(model_modifier && other) = default;

    ~model_modifier()
    {
        // Unlock mutex before triggering the notifications, otherwise we would
        // cause a deadlock, because mutex is accessed during notification handling
        m_model_ref.m_mutex.unlock();

        m_model_ref.trigger();
    }

    /// \brief      Modifiable model value.
    T & value()
    {
        return m_model_ref.m_value;
    }

private:
    explicit model_modifier(model<T> & model_ref) :
        m_model_ref(model_ref)
    {
        // m_model_ref.m_lock needs to be already locked here by model<T>
    }

private:
    /// \brief      Modified model reference.
    model<T> & m_model_ref;
};

#pragma once

/// \brief      Ensures thread-safe unique access of `model`.
template<typename T>
class model_accessor
{
public:
    template<typename TT>
    friend class model;

public:
    model_accessor(const model_accessor & other) :
        m_model_ref(other.m_model_ref)
    {
    }

    model_accessor(model_accessor && other) :
        m_model_ref(other.m_model_ref)
    {
    }

    ~model_accessor()
    {
        m_model_ref.m_mutex.unlock();
    }

    /// \brief      Readable model value.
    const T & value() const
    {
        return m_model_ref.m_value;
    }

private:
    /// \brief      
    explicit model_accessor(const model<T> & model_ref) :
        m_model_ref(model_ref)
    {
        // m_model_ref.m_lock needs to be already locked here by model<T>
    }

private:
    /// \brief      Accessed model reference.
    const model<T> & m_model_ref;
};
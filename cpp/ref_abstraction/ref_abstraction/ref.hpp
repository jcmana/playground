template<typename T>
class ref
{
public:
    ref(T * ptr) :
        m_ptr(ptr)
    {
    }

    ref(std::nullptr_t) = delete;

    T * get()
    {
        return m_ptr;
    }

    operator  T() const
    {
        return (*m_ptr);
    }

    operator  T *() const
    {
        return m_ptr;
    }

    T * operator ->()
    {
        return m_ptr;
    }

    const T * get() const
    {
        return m_ptr;
    }

    const T * operator ->() const
    {
        return m_ptr;
    }

private:
    T * m_ptr;
};
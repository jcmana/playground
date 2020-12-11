#pragma once

#include <mutex>

using T = int;

class observable
{
public:
    operator const T & () const
    {
        return m_value;
    }

private:
    T m_value;
};
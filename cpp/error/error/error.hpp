#pragma once

/// <summary>Unique error identification.</summary>
struct error 
{
    unsigned long  data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char  data4[8];
};

/// <summary>Compares `error` for equality.</summary>
inline bool operator ==(const error & lhs, const error & rhs)
{
    if (lhs.data1 != rhs.data1) return false;
    if (lhs.data2 != rhs.data2) return false;
    if (lhs.data3 != rhs.data3) return false;

    for (auto it_lhs = lhs.data4, it_rhs = rhs.data4; it_lhs != lhs.data4 + 8; ++it_lhs, ++it_rhs)
    {
        if (*it_lhs != *it_rhs) return false;
    }

    return true;
}

/// <summary>Compares `error` for inequality.</summary>
inline bool operator !=(const error & lhs, const error & rhs)
{
    if (lhs.data1 == rhs.data1) return false;
    if (lhs.data2 == rhs.data2) return false;
    if (lhs.data3 == rhs.data3) return false;

    for (auto it_lhs = lhs.data4, it_rhs = rhs.data4; it_lhs != lhs.data4 + 8; ++it_lhs, ++it_rhs)
    {
        if (*it_lhs == *it_rhs) return false;
    }

    return true;
}

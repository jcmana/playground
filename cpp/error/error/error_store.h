#pragma once

#include <cassert>
#include <unordered_map>
#include <utility>

#include "error.h"

/// <summary>Pairs `error` with its description.</summary>
template<typename T>
class error_store
{
public:
    /// <summary>Constructor, initializes store with unknown `error` description.</summary>
    error_store(T unknown) :
        m_unknown_error(std::move(unknown))
    {
    }

    /// <summary>Store an `error` code with its description.</summary>
    void set(error code, T detail)
    {
        m_store.emplace(code, std::move(detail));
    }

    /// <summary>Retrieve description for an `error`.</summary>
    const T & get(error code) const
    {
        const auto it = m_store.find(code);

        if (it == m_store.end())
        {
            return m_unknown_error;
        }

        return it->second;
    }

private:
    struct hash
    {
        size_t operator ()(const error & e) const
        {
            return e.data1 + e.data2 + e.data3;
        }
    };

private:
    std::unordered_map<error, T, hash> m_store;

    // Unknown error description
    T m_unknown_error = {};
};

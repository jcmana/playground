#pragma once

#include <cstdint>
#include <vector>
#include <numeric>
#include <exception>

/// \brief      Representation of an execution progress split into parts.
struct structured_progress
{
    /// \brief      
    struct part
    {
        constexpr part() :
            size(1),
            value()
        {
        }

        constexpr part(std::uint32_t size) :
            size(size),
            value()
        {
        }

        constexpr double percentage() const
        {
            return double(value) / double(size);
        }

        const std::uint32_t size;
        std::uint32_t value;
    };

    inline double percentage() const
    {
        std::uint64_t sum_value = 0;
        std::uint64_t sum_size = 0;

        for (const auto & part : parts)
        {
            sum_value += part.value;
            sum_size += part.size;
        }

        return double(sum_value) / double(sum_size);
    }

    std::vector<part> parts;
};
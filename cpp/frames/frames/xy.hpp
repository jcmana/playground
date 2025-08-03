#pragma once

#include "space_default.hpp"

/// \brief      Coordinates in `Space` and `Scalar` representation.
template<typename Space, typename Scalar>
struct xy;

template<typename Space, typename Scalar>
struct xy
{
    template<typename OtherSpace, typename OtherScalar>
    operator xy<OtherSpace, OtherScalar>() const;
    /*
    {
        static_assert(false, "operator not implemented");
    }
    */

    Scalar x;
    Scalar y;
};

/// \brief      Local coordinates in a `frame` in `void` space.
/// 
/// This type is bridge between strong-typed `xy`, which ensures using only
/// coordinates in compatible spaces, and between weak-typed `xy`, which depends
/// on programmer's knowledge what frame and space it belongs to.
template
struct xy<void, double>;

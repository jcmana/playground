#pragma once

#include <initializer_list>

#include "space_default.hpp"

/// \brief      Coordinates in `Space` and `Scalar` representation.
template<typename Space, typename Scalar>
struct xy;

template<typename SourceSpace, typename SourceScalar, typename TargetSpace, typename TargetScalar>
xy<TargetSpace, TargetScalar> transform_implicit(const xy<SourceSpace, SourceScalar> & source);

template<typename Space, typename Scalar>
struct xy
{
    xy() :
        xy(Scalar(), Scalar())
    {
    }

    xy(Scalar x, Scalar y) :
        x(x),
        y(y)
    {
    }

    xy(const xy & other) = default;
    xy(xy && other) = default;

    xy & operator =(const xy & other) = default;
    xy & operator =(xy && other) = default;

    template<typename OtherSpace, typename OtherScalar>
    xy(const xy<OtherSpace, OtherScalar> & other) :
        xy(transform_implicit<OtherSpace, OtherScalar, Space, Scalar>(other))
    {
    }

    template<typename OtherSpace, typename OtherScalar>
    xy & operator =(const xy<OtherSpace, OtherScalar> & other)
    {
        (*this) = transform_implicit<OtherSpace, OtherScalar, Space, Scalar>(other);
        return (*this);
    }

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

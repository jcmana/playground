#pragma once

template<typename T>
struct deduction
{
    deduction(const T & value) :
        t(value)
    {}

    deduction(T && value) :
        t(value)
    {}

    T t;
};

deduction(int) -> deduction<double>;

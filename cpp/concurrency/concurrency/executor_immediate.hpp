#pragma once

/// \brief      Executes posted tasks immediately and synchronously.
template<typename T>
class executor_immediate
{
public:
    /// \brief      Posts tasks for immediate execution.
    template<typename F>
    T post(F && f)
    {
        return f();
    }
};
#pragma once

class IWaitable
{
public:
    virtual ~IWaitable() = default;

    /// \brief      Blocks until this object modification.
    virtual void Wait() = 0;
};
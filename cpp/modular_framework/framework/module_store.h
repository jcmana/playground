#pragma once

#include <memory>
#include <vector>

#include "module_store_intf.h"
#include "module_intf.h"

class module_store : public module_store_intf
{
public:
    module_store()
    {
    }

    void load_modules()
    {
        for (auto & up_module : m_modules)
        {
            //up_module->load(...);
        }
    }

public:
    virtual void add(module_intf * ptr) override
    {
        m_modules.push_back(std::unique_ptr<module_intf>(ptr));
    }

    virtual void remove(module_intf * ptr) override
    {
        // JMTODO:
    }

private:
    std::vector<std::unique_ptr<module_intf>> m_modules;
};
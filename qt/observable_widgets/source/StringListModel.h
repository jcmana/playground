#pragma once

#include <vector>
#include <string>
#include <optional>

class StringListModel
{
public:
    using Container = std::vector<std::string>;

public:
    StringListModel()
    {
    }

    // ...

private:
    Container m_data;
};
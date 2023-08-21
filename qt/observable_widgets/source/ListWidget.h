#pragma once

#include <vector>
#include <string>

#include <QtWidgets/QListWidget>

#include "observable.hpp"

class ListWidget :
    public QListWidget
{
public:
    using StringList = std::vector<std::string>;

public:
    ListWidget(QWidget * parent = nullptr) :
        QListWidget(parent)
    {
        auto updateListWidget = [this](const auto & lines)
        {
            clear();

            for (const auto & line : lines)
            {
                addItem(QString::fromUtf8(line.c_str()));
            }
        };
        soLines.observe(updateListWidget);
    }

public:
    shared_obe<StringList> soLines;
};
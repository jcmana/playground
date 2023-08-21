#pragma once

#include <any>
#include <vector>
#include <string>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QComboBox>

#include "observable.hpp"

class ComboBox :
    public QComboBox
{
public:
    using Items = std::vector<std::string>;
    using ItemsIndex = Items::size_type;

public:
    ComboBox(QWidget * parent = nullptr) :
        QComboBox(parent)
    {
        connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onCurrentIndexChanged);
    }

    void reset(shared_obe<Items> soItems)
    {
        m_soItems = std::move(soItems);
        m_soItems.observe(&ComboBox::onItemsChanged, this);
    }

    void reset(shared_obe<ItemsIndex> soItemsIndex)
    {
        m_soItemsIndex = std::move(soItemsIndex);
        m_soItemsIndex.observe(&ComboBox::onItemsIndexChanged, this);
    }

private:
    void onItemsChanged(const Items & items)
    {
        auto update = [this, items]
        {
            QSignalBlocker blocker(this);

            clear();
            for (const auto & label : items)
            {
                addItem(QString::fromUtf8(label.c_str()));
            }
        };
        QMetaObject::invokeMethod(this, update);
    }

    void onItemsIndexChanged(const ItemsIndex & selected)
    {
        auto update = [this, selected]
        {
            QSignalBlocker blocker(this);

            setCurrentIndex(static_cast<int>(selected));
        };
        QMetaObject::invokeMethod(this, update);
    }

    void onCurrentIndexChanged(int index)
    {
    }

private:
    shared_obe<Items> m_soItems;
    shared_obe<ItemsIndex> m_soItemsIndex;
};

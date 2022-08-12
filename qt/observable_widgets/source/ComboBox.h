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
        m_soItems.observe(&ComboBox::OnItemsChanged, this);
        m_soSelected.observe(&ComboBox::OnSelectedItemChanged, this);

    }

    shared_obe<Items> soItems()
    {
        return m_soItems;
    }

    shared_obe<const Items> soItems() const
    {
        return m_soItems;
    }

    shared_obe<ItemsIndex> soSelectedItem()
    {
        return m_soSelected;
    }

    shared_obe<const ItemsIndex> soSelectedItem() const
    {
        return m_soSelected;
    }

private:
    void OnItemsChanged(const Items & items)
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

    void OnSelectedItemChanged(const ItemsIndex & selected)
    {
        auto update = [this, selected]
        {
            QSignalBlocker blocker(this);

            setCurrentIndex(static_cast<int>(selected));
        };
        QMetaObject::invokeMethod(this, update);
    }

    void OnCurrentIndexChanged(int index)
    {

        
    }

private:
    shared_obe<Items> m_soItems;
    shared_obe<ItemsIndex> m_soSelected;
};

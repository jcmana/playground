#pragma once

#include <vector>
#include <string>
#include <algorithm>

class input_select_model
{
public:
    using item_label = std::string;
    using item_index = std::size_t;

    struct item
    {
        item_label label;
        bool selected;
    };

    using container = std::vector<item>;

public:
    bool select(container::const_iterator it)
    {
        if (it == m_items.cend())
        {
            return false;
        }

        for (auto it_mutable = m_items.begin(); it_mutable != m_items.end(); ++it_mutable)
        {
            if (it_mutable == it)
            {
                it_mutable->selected = true;
            }
            else
            {
                it_mutable->selected = false;
            }
        }

        return true;
    }

    bool select(item_label label)
    {
        bool selected = false;
        for (auto & item : m_items)
        {
            item.selected = false;

            if (item.label == label)
            {
                item.selected = true;
                selected = true;
            }
        }

        return selected;

        auto it = std::find_if(m_items.begin(), m_items.end(), [&label](const auto & item) { return item.label == label; });

        if (it == m_items.end())
        {
            return false;
        }

        return select(it);
    }

    void select(item_index index)
    {
        for (auto & item : m_items)
        {
            item.selected = false;
        }

        m_items[index].selected = true;
    }

    bool add(item_label label)
    {
        auto pred = [&label](const item & current_item)
        {
            return current_item.label == label;
        };
        auto it = std::find_if(m_items.begin(), m_items.end(), pred);

        if (it != m_items.end())
        {
            return false;
        }

        item i;
        i.label = std::move(label);
        i.selected = false;

        m_items.push_back(i);
        return true;
    }

    bool remove(item_label label)
    {
        if (m_items.empty())
        {
            return false;
        }

        auto it = m_items.begin();

        for (; it != m_items.end(); ++it)
        {
            if (it->label == label)
            {
                break;
            }
        }

        if (it == m_items.end())
        {
            return false;
        }

        if (it->selected == false)
        {
            return true;
        }

        auto it_next = std::next(it);
        if (it_next != m_items.end())
        {
            it_next->selected = true;
            return true;
        }

        auto it_prev = std::prev(it);
        it_prev->selected = true;
        return true;
    }

    void remove(item_index index);

    void rename(item_index index, item_label label);

    std::size_t size() const
    {
        return m_items.size();
    }

    container::const_iterator begin() const
    {
        return m_items.cbegin();
    }

    container::const_iterator end() const
    {
        return m_items.cend();
    }

    const item & at(std::size_t index) const
    {
        return m_items.at(index);
    }

private:
    container m_items;
};

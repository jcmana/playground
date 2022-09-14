/// This bind selection together with data. If we take some inspiration from Qt in more complex views and models
/// the approach is to split these two. This way we allow to have multiple views with different selections which
/// is kinda cool.

#pragma once

#include <vector>
#include <string>
#include <algorithm>

class input_select_state
{
public:
    using item_label = std::string;
    using item_index = std::size_t;

    struct item
    {
        item_label label;
        bool selected;
    };

public:
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

    std::vector<item>::const_iterator begin() const
    {
        return m_items.cbegin();
    }

    std::vector<item>::const_iterator end() const
    {
        return m_items.cend();
    }

    const item & at(std::size_t index) const
    {
        return m_items.at(index);
    }

private:
    std::vector<item> m_items;
};
#include <iostream>
#include <vector>
#include <optional>

#include "observable.hpp"
#include "breaking_circular_observers.h"

void breaking_circular_observers()
{
    shared_obe<std::vector<int>> so_item_list;
    shared_obe<std::optional<std::size_t>> so_selected_item_index;
    shared_obe<std::optional<int>> so_selected_item;

    /*
    auto on_item_list_changed = [so_selected_item_index](const auto & item_list) mutable
    {
        std::optional<std::size_t> selected_item_index;

        if (item_list.empty())
        {
            selected_item_index = std::nullopt;
        }
        else
        {
            selected_item_index = 0;
        }

        unique_txn{so_selected_item_index} = selected_item_index;
    };
    so_item_list.observe(on_item_list_changed);
    */

    auto on_selected_item_index_changed = [so_item_list, so_selected_item](const auto & selected_item_index) mutable
    {
        if (selected_item_index.has_value() == false)
        {
            return;
        }

        const auto selected_item = shared_txn{so_item_list}.get().at(selected_item_index.value());
        unique_txn{so_selected_item} = selected_item;
    };
    so_selected_item_index.observe(on_selected_item_index_changed);

    auto on_selected_item_changed = [so_item_list, so_selected_item_index](const auto & selected_item) mutable
    {
        if (selected_item.has_value() == false)
        {
            return;
        }

        unique_txn tx_item_list{so_item_list};
        shared_txn tx_selected_item_index{so_selected_item_index};
        tx_item_list.get().at(tx_selected_item_index.get().value()) = selected_item.value();
    };
    so_selected_item.observe(on_selected_item_changed);

    unique_txn{so_item_list}.get() = {7, 4};
    unique_txn{so_selected_item_index}.get() = 0;

    shared_txn tx_selected_item{so_selected_item};
    if (tx_selected_item.get().has_value())
    {
        std::cout << tx_selected_item.get().value() << std::endl;
    }
    else
    {
        std::cout << "-" << std::endl;
    }
}

void breaking_circular_observers2()
{
    class state
    {
    public:
        void change_list(const std::vector<int> & value)
        {
            m_list = value;
            m_selected_index = std::nullopt;
            m_selected = std::nullopt;
        }

        void change_selected_index(const std::optional<std::size_t> & selected_index)
        {
            m_selected_index = selected_index;

            if (m_selected_index.has_value())
            {
                m_selected = m_list.at(m_selected_index.value());
            }
        }

        void change_selected(const std::optional<int> & selected)
        {
            m_selected = selected;

            if (m_selected.has_value())
            {
                m_list.at(m_selected_index.value()) = m_selected.value();
            }
        }

        auto selected() const
        {
            return m_selected;
        }

    private:
        std::vector<int> m_list;
        std::optional<std::size_t> m_selected_index;
        std::optional<int> m_selected;
    };

    shared_obe<state> so_state;
    shared_obe<std::optional<int>> so_selected;

    auto forward_state_selected = [so_selected](const auto & value) mutable
    {
        unique_txn{so_selected} = value.selected();
    };
    so_state.observe(forward_state_selected);

    auto consume_selected = [](const auto & value)
    {
        if (value.has_value())
        {
            std::cout << value.value() << std::endl;
        }
    };
    so_selected.observe(consume_selected);

    unique_txn tx_state{so_state};
    tx_state.get().change_list({7, 4});
    tx_state.get().change_selected_index(0);
    tx_state.get().change_selected_index(1);
}
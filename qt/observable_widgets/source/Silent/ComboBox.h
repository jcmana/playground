#pragma once

#include <optional>
#include <vector>
#include <string>

#include <QtWidgets/QComboBox>

namespace Silent
{

/// \brief      Combobox which doesn't have internal selection logic.
class ComboBox :
    public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget * parent = nullptr);

    /// \returns    Index of selected item.
    /// \note       Index might be out-of-bounds.
    std::optional<std::size_t> currentIndex() const;

signals:
    /// \brief      Emitted only when user selects an item.
    void currentIndexChanged(std::optional<std::size_t> index);

public slots:
    /// \brief      Clears all items.
    /// \note       Doesn't change current index.
    void clearItems();

    /// \brief      Inserts items with `labels` before `index`.
    void insertItems(std::size_t index, const std::vector<std::string> & labels);
    /// \brief      Inserts item with `label` before `index`.
    void insertItem(std::size_t index, const std::string & label);

    /// \brief      Adds items with `labels` to the end.
    void addItems(const std::vector<std::string> & labels);
    /// \brief      Adds item with `labels` to the end.
    void addItem(const std::string & label);

    /// \brief      Changes selected item `index`.
    void setCurrentIndex(std::optional<std::size_t> index);

private:
    void onCurrentIndexChanged(int index);

private:
    std::optional<std::size_t> m_currentIndex;
};

};
#include "ComboBox.h"

namespace Silent
{

ComboBox::ComboBox(QWidget * parent) :
    QComboBox(parent)
{
    connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onCurrentIndexChanged);
}

std::optional<std::size_t> ComboBox::currentIndex() const
{
    return m_currentIndex;
}

void ComboBox::clearItems()
{
    QSignalBlocker blocker(this);
    QComboBox::clear();
    setCurrentIndex(m_currentIndex);
}

void ComboBox::insertItems(std::size_t index, const std::vector<std::string> & labels)
{
    QSignalBlocker blocker(this);
    for (const auto & label : labels)
    {
        QComboBox::insertItem(static_cast<int>(index++), label.c_str());
    }
    setCurrentIndex(m_currentIndex);
}

void ComboBox::insertItem(std::size_t index, const std::string & label)
{
    QSignalBlocker blocker(this);
    QComboBox::insertItem(static_cast<int>(index), label.c_str());
    setCurrentIndex(m_currentIndex);
}

void ComboBox::addItems(const std::vector<std::string> & labels)
{
    QSignalBlocker blocker(this);
    for (const auto & label : labels)
    {
        QComboBox::addItem(label.c_str());
    }
    setCurrentIndex(m_currentIndex);
}

void ComboBox::addItem(const std::string & label)
{
    QSignalBlocker blocker(this);
    QComboBox::addItem(label.c_str());
    setCurrentIndex(m_currentIndex);
}

void ComboBox::setCurrentIndex(std::optional<std::size_t> index)
{
    m_currentIndex = index;

    if (m_currentIndex.has_value())
    {
        QComboBox::setCurrentIndex(static_cast<int>(m_currentIndex.value()));
    }
    else
    {
        QComboBox::setCurrentIndex(-1);
    }
}

void ComboBox::onCurrentIndexChanged(int index)
{
    // Block changing displayed current item
    {
        QSignalBlocker blocker(this);
        setCurrentIndex(m_currentIndex);
    }

    if (index == -1)
    {
        emit currentIndexChanged(std::nullopt);
    }
    else
    {
        emit currentIndexChanged(static_cast<std::size_t>(index));
    }
}

};
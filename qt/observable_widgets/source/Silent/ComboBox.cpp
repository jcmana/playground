#include "ComboBox.h"

namespace Silent
{

ComboBox::ComboBox(QWidget * parent) :
    QComboBox(parent)
{
    connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onCurrentIndexChanged);
}

int ComboBox::currentIndex() const
{
    return m_currentIndex;
}

void ComboBox::clear()
{
    QSignalBlocker blocker(this);
    QComboBox::clear();
    QComboBox::setCurrentIndex(m_currentIndex);
}

void ComboBox::insertItem(int index, const QString & text)
{
    QSignalBlocker blocker(this);
    QComboBox::insertItem(index, text);
    QComboBox::setCurrentIndex(m_currentIndex);
}

void ComboBox::addItem(const QString & text)
{
    QSignalBlocker blocker(this);
    QComboBox::addItem(text);
    QComboBox::setCurrentIndex(m_currentIndex);
}

void ComboBox::setCurrentIndex(int index)
{
    m_currentIndex = index;
    QComboBox::setCurrentIndex(m_currentIndex);
}

void ComboBox::onCurrentIndexChanged(int index)
{
    // Block changing displayed current item
    {
        QSignalBlocker blocker(this);
        QComboBox::setCurrentIndex(m_currentIndex);
    }

    emit currentIndexChanged(index);
}

};
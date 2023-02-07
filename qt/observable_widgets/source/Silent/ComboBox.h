#pragma once

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
    int currentIndex() const;

signals:
    /// \brief      Emitted when user selects an item.
    void currentIndexChanged(int index);

public slots:
    void clear();
    void insertItem(int index, const QString & text);
    void addItem(const QString & text);
    void setCurrentIndex(int index);

private:
    void onCurrentIndexChanged(int index);

private:
    int m_currentIndex;
};

};
#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include "shared_obe.hpp"

namespace GUI
{

class Label :
    public QWidget
{
public:
    Label(QWidget * parent = nullptr) :
        QWidget(parent)
    {
        m_pLayout = new QVBoxLayout(this);
        m_pLabel = new QLabel();
        m_pLayout->addWidget(m_pLabel);
        setLayout(m_pLayout);
    }

private:
    QVBoxLayout * m_pLayout;
    QLabel * m_pLabel;
};

}
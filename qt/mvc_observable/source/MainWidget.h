#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpacerItem>

#include "shared_observable.hpp"

#include "State.h"

class MainWidget :
    public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget * parent = nullptr) :
        QWidget(parent)
    {
        m_pSpinBoxQuestion = new QSpinBox();
        m_pSpinBoxQuestion->setMinimum(0);
        m_pSpinBoxQuestion->setMaximum(100);

        m_pButton = new QPushButton();
        m_pButton->setText("Compute the answer to universe");

        m_pButtonTerminate = new QPushButton();
        m_pButtonTerminate->setText("X");

        m_pSpinBoxAnswer = new QSpinBox();
        m_pSpinBoxAnswer->setMinimum(0);
        m_pSpinBoxAnswer->setMaximum(100'000);
        m_pSpinBoxAnswer->setReadOnly(true);

        auto pSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        auto pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pSpinBoxQuestion, 0, 0);
        pLayout->addWidget(m_pButton, 0, 1);
        pLayout->addWidget(m_pButtonTerminate, 0, 2);
        pLayout->addWidget(m_pSpinBoxAnswer, 1, 0, 1, 3);
        pLayout->addItem(pSpacer, 2, 0, 1, 3);
        setLayout(pLayout);

        connect(m_pButton, &QAbstractButton::clicked, this, [this]{ emit buttonClicked(true); });
        connect(m_pButtonTerminate, &QAbstractButton::clicked, this, [this]{ emit buttonTerminateClicked(true); });
        connect(m_pSpinBoxQuestion, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::questionChanged);
    }

signals:
    void buttonClicked(bool);
    void buttonTerminateClicked(bool);

    void questionChanged(int);

public slots:
    void setCompuationRunning(bool running)
    {
        m_pSpinBoxQuestion->setEnabled(!running);
        m_pSpinBoxAnswer->setEnabled(!running);
        m_pButton->setEnabled(!running);
        m_pButtonTerminate->setEnabled(running);
    }

    void setAnswer(int answer)
    {
        m_pSpinBoxAnswer->setValue(answer);
    }

private:
    QSpinBox * m_pSpinBoxQuestion;
    QSpinBox * m_pSpinBoxAnswer;
    QPushButton * m_pButton;
    QPushButton * m_pButtonTerminate;
};
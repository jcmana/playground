#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpacerItem>

class MainWidget :
    public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget * parent = nullptr)
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

        connect(m_pSpinBoxQuestion, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::questionChanged);
        connect(m_pButton, &QAbstractButton::clicked, this, &MainWidget::answerRequested);
        connect(m_pButtonTerminate, &QAbstractButton::clicked, this, &MainWidget::terminateRequested);
    }

signals:
    void questionChanged(int value);
    void answerRequested();
    void terminateRequested();

public slots:
    void setAnswer(int value)
    {
        m_pSpinBoxAnswer->setValue(value);
    }

    void setRunning()
    {
        m_pSpinBoxQuestion->setEnabled(false);
        m_pSpinBoxAnswer->setEnabled(false);
        m_pButton->setEnabled(false);
        m_pButtonTerminate->setEnabled(true);
    }

    void setDone()
    {
        m_pSpinBoxQuestion->setEnabled(true);
        m_pSpinBoxAnswer->setEnabled(true);
        m_pButton->setEnabled(true);
        m_pButtonTerminate->setEnabled(false);
    }

private:
    QSpinBox * m_pSpinBoxQuestion;
    QSpinBox * m_pSpinBoxAnswer;
    QPushButton * m_pButton;
    QPushButton * m_pButtonTerminate;
};
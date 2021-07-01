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

        m_pSpinBoxAnswer = new QSpinBox();
        m_pSpinBoxAnswer->setMinimum(0);
        m_pSpinBoxAnswer->setMaximum(100'000);
        m_pSpinBoxAnswer->setReadOnly(true);

        auto pSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        auto pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pSpinBoxQuestion, 0, 0);
        pLayout->addWidget(m_pButton, 0, 1);
        pLayout->addWidget(m_pSpinBoxAnswer, 1, 0, 1, 2);
        pLayout->addItem(pSpacer, 2, 0, 1, 2);
        setLayout(pLayout);

        connect(m_pSpinBoxQuestion, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::questionChanged);
        connect(m_pButton, &QAbstractButton::clicked, this, &MainWidget::answerRequested);
    }

signals:
    void questionChanged(int value);
    void answerRequested();

public slots:
    void setAnswer(int value)
    {
        m_pSpinBoxAnswer->setValue(value);
    }

private:
    QSpinBox * m_pSpinBoxQuestion;
    QPushButton * m_pButton;
    QSpinBox * m_pSpinBoxAnswer;
};
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
    MainWidget(
        shared_observable<State> state,
        shared_observable<int> question, 
        shared_observable<int> answer,
        shared_observable<bool> compute,
        shared_observable<bool> terminate,
        QWidget * parent = nullptr) :
        QWidget(parent),
        m_state(state),
        m_question(question),
        m_answer(answer),
        m_compute(compute),
        m_terminate(terminate)
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

        connect(m_pSpinBoxQuestion, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWidget::OnQuestionChanged);
        connect(m_pButton, &QAbstractButton::clicked, this, &MainWidget::OnButtonClicked);
        connect(m_pButtonTerminate, &QAbstractButton::clicked, this, &MainWidget::OnButtonTerminateClicked);

        // Connect observables:
        m_state.observe(std::bind(&MainWidget::OnStateChanged, this, std::placeholders::_1));
        m_answer.observe(std::bind(&MainWidget::OnAnswerChanged, this, std::placeholders::_1));
    }

public slots:
    void setState(State state)
    {
        switch (state)
        {
            case State::IDLE:
            case State::FINISHED:
            case State::TERMINATED:
                m_pSpinBoxQuestion->setEnabled(true);
                m_pSpinBoxAnswer->setEnabled(true);
                m_pButton->setEnabled(true);
                m_pButtonTerminate->setEnabled(false);
                break;

            case State::RUNNING:
                m_pSpinBoxQuestion->setEnabled(false);
                m_pSpinBoxAnswer->setEnabled(false);
                m_pButton->setEnabled(false);
                m_pButtonTerminate->setEnabled(true);
                break;
        }
    }

    void setAnswer(int answer)
    {
        m_pSpinBoxAnswer->setValue(answer);
    }

private slots:
    void OnQuestionChanged(int question)
    {
        unique_txn{m_question} = question;
    }

    void OnButtonClicked()
    {
        unique_txn{m_compute} = true;
    }

    void OnButtonTerminateClicked()
    {
        unique_txn{m_terminate} = true;
    }

private:
    void OnStateChanged(State state)
    {
        QMetaObject::invokeMethod(this, std::bind(&MainWidget::setState, this, state));
    }

    void OnAnswerChanged(int answer)
    {
        QMetaObject::invokeMethod(this, std::bind(&MainWidget::setAnswer, this, answer));
    }

private:
    shared_observable<State> m_state;
    shared_observable<int> m_question; 
    shared_observable<int> m_answer;

    shared_observable<bool> m_compute;
    shared_observable<bool> m_terminate;

    QSpinBox * m_pSpinBoxQuestion;
    QSpinBox * m_pSpinBoxAnswer;
    QPushButton * m_pButton;
    QPushButton * m_pButtonTerminate;
};
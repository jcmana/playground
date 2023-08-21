#pragma once

#include <functional>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>

#include "observable.hpp"

namespace GUI
{

class SpinBox :
    public QSpinBox
{
public:
    SpinBox(shared_obe<int> model, QWidget * parent = nullptr) :
        QSpinBox(parent),
        m_model(model)
    {
        connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpinBox::OnViewChanged);
        m_model.observe(&SpinBox::OnModelChanged, this);
    }

private:
    void OnModelChanged(const int & value)
    {
        auto functor = [this, value]
        {
            QSignalBlocker blocker(this);
            setValue(value);
        };
        QMetaObject::invokeMethod(this, functor);
    }

    void OnViewChanged(int value)
    {
        unique_txn{m_model} = value;
    }

private:
    shared_obe<int> m_model;
};

};
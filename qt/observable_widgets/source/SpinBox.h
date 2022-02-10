#pragma once

#include <functional>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>

#include "shared_observable.hpp"

namespace GUI
{

class SpinBox :
    public QSpinBox
{
     Q_OBJECT

public:
    SpinBox(shared_observable<int> model, QWidget * parent = nullptr) :
        QSpinBox(parent),
        m_model(model)
    {
        connect(this, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpinBox::OnViewChanged);
        m_model.observe(std::bind(&SpinBox::OnModelChanged, this, std::placeholders::_1));
    }

private:
    void OnModelChanged(int value)
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
    shared_observable<int> m_model;
};

};
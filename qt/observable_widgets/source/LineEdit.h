#pragma once

#include <string>
#include <functional>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "observable.hpp"

namespace GUI
{

class LineEdit :
    public QLineEdit
{
public:
    LineEdit(QWidget * parent = nullptr) :
        QLineEdit(parent)
    {
        connect(this, &QLineEdit::textChanged, this, &LineEdit::OnViewChanged);
        m_model.observe(std::bind(&LineEdit::OnModelChanged, this, std::placeholders::_1));
    }

    shared_obe<std::string> model()
    {
        return m_model;
    }

private:
    void OnModelChanged(const std::string & value)
    {
        auto functor = [this, value]
        {
            QSignalBlocker blocker(this);
            setText(value.c_str());
        };
        QMetaObject::invokeMethod(this, functor);
    }

    void OnViewChanged(const QString & value)
    {
        unique_txn{m_model} = value.toStdString();
    }

private:
    shared_obe<std::string> m_model;
};

};
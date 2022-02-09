#pragma once

#include <string>
#include <functional>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "shared_observable.hpp"

namespace GUI
{

class LineEdit :
    public QWidget
{
    Q_OBJECT

public:
    LineEdit(shared_observable<std::string> model, QWidget * parent = nullptr) :
        QWidget(parent),
        m_model(model)
    {
        m_lineEdit = new QLineEdit(this);
        connect(m_lineEdit, &QLineEdit::textChanged, this, &LineEdit::OnLineEditChanged);
        m_model.observe(std::bind(&LineEdit::OnModelChanged, this, std::placeholders::_1));
    }

    shared_observable<std::string> model()
    {
        return m_model;
    }

private:
    void OnModelChanged(const std::string & value)
    {
        QSignalBlocker blocker(m_lineEdit);
        m_lineEdit->setText(value.c_str());
    }

    void OnLineEditChanged(const QString & value)
    {
        unique_txn{m_model} = value.toStdString();
    }

private:
    shared_observable<std::string> m_model;
    QLineEdit * m_lineEdit;
};

};
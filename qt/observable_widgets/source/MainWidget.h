#pragma once

#include <limits>

#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>

#include "SpinBox.h"
#include "LineEdit.h"

namespace GUI
{

class MainWidget :
    public QWidget
{
    Q_OBJECT

public:
    MainWidget(
        shared_observable<int> idModel, 
        shared_observable<std::string> nameModel,
        shared_observable<std::string> usernameModel,
        QWidget * parent = nullptr) :
        QWidget(parent)
    {
        auto * idSpinBox = new SpinBox(std::move(idModel));
        idSpinBox->setMinimum(0);
        idSpinBox->setMaximum(std::numeric_limits<int>::max());
        auto * nameLineEdit = new LineEdit(std::move(nameModel));
        auto * usernameLineEdit = new LineEdit(std::move(usernameModel));
        usernameLineEdit->setReadOnly(true);

        auto * layout = new QGridLayout(this);
        layout->addWidget(new QLabel("id: "), 0, 0);
        layout->addWidget(idSpinBox, 0, 1);
        layout->addWidget(new QLabel("name: "), 1, 0);
        layout->addWidget(nameLineEdit, 1, 1);
        layout->addWidget(new QLabel("username: "), 2, 0);
        layout->addWidget(usernameLineEdit, 2, 1);
        setLayout(layout);

        setMinimumSize(200, 100);
    }
};

};
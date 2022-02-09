#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QApplication>

#include "shared_observable.hpp"
#include "LineEdit.h"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    auto observer = [](const std::string & value)
    {
        std::cout << value << std::endl;
    };

    shared_observable<std::string> model;
    model.observe(observer);
    
    GUI::LineEdit e(model);
    e.show();

    unique_txn{model} = "hovno";

    return application.exec();
}
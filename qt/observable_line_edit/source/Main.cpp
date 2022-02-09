#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QApplication>

#include "shared_observable.hpp"
#include "LineEdit.h"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    shared_observable<std::string> model_name;
    shared_observable<int> model_id;
    shared_observable<std::string> model_text;

    auto combine = [model_text](const std::string & name, int id)
    {
        unique_txn{model_text} = name + std::to_string(id);
    };
    join(combine, model_name, model_id);

    auto log = [](const std::string & text)
    {
        std::cout << text << std::endl;
    };
    model_text.observe(log);

    GUI::LineEdit e(model_name);
    e.show();

    unique_txn{model_name} = "idiot";
    unique_txn{model_id} = 7;

    return application.exec();
}
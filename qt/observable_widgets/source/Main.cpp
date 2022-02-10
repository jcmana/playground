#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QApplication>

#include "shared_observable.hpp"

#include "MainWidget.h"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Shared models between programming layers:
    shared_observable<std::string> nameModel;
    shared_observable<int> idModel;
    shared_observable<std::string> usernameModel;

    // Model data and control flow logic:
    auto combine = [usernameModel](const std::string & name, int id)
    {
        unique_txn{usernameModel} = name + " " + std::to_string(id);
    };
    join(combine, nameModel, idModel);

    auto log = [](const std::string & text)
    {
        std::cout << text << std::endl;
    };
    usernameModel.observe(log);

    // GUI setup with dependency-injected shared models:
    GUI::MainWidget e(idModel, nameModel, usernameModel);
    e.show();

    // Initialize models with default values:
    unique_txn{nameModel} = "batman";
    unique_txn{idModel} = 707;

    return application.exec();
}
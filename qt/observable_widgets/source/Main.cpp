#include <iostream>

#include <QtWidgets/QApplication>

#include "UserAccountLogic.h"
#include "MainWidget.h"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Shared models and logic between programming layers:
    UserAccountLogic logic;

    auto logUsername = [](const std::string & username)
    {
        std::cout << username << std::endl;
    };
    logic.usernameModel.observe(logUsername);

    // GUI setup with dependency-injected shared models:
    GUI::MainWidget e(logic.idModel, logic.nameModel, logic.usernameModel);
    e.show();

    // Initialize models with default values:
    unique_txn{logic.nameModel} = "batman";
    unique_txn{logic.idModel} = 707;

    return application.exec();
}
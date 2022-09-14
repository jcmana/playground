#include <iostream>
#include <thread>
#include <future>

#include <QtWidgets/QApplication>

#include "ComboBox.h"
#include "SpinBox.h"
#include "ListWidget.h"
#include "InputSelect.h"

#include "input_select_model.hpp"

#include "observable.hpp"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Shared models and logic between programming layers:
    //UserAccountLogic logic;

    /*
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
    */

    /*
    ListWidget widget;
    widget.show();

    auto fill = [&widget]
    {
        {
            unique_txn{widget.soLines}.get().push_back("hovno");
        }        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            unique_txn{widget.soLines}.get().push_back("jebat");
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            unique_txn{widget.soLines}.get() =
            { 
                "wtf",
                "bbq",
                "hemenex",
            };
        }

    };
    auto wtf = std::async(fill);
    */

    /*
    ComboBox combo;
    combo.show();

    auto soItems = combo.soItems();
    unique_txn{soItems} = {"a", "b", "c", "d"};
    auto soSelected = combo.soSelectedItem();
    unique_txn{soSelected} = 5;
    */

    InputSelect is;
    is.show();

    {
        auto so = is.soState();
        unique_txn tx{so};
        tx.get().add("a");
        tx.get().add("b");
        tx.get().add("c");
        tx.get().add("d");
        tx.get().select("c");
    }

    return application.exec();
}
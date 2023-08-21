#include <iostream>
#include <thread>
#include <future>

#include <QtWidgets/QApplication>

//#include "ComboBox.h"
//#include "SpinBox.h"
//#include "ListWidget.h"
//#include "InputSelect.h"

//#include "input_select_model.hpp"

//#include "NonIntrusive/ComboBox.h"
//#include "NonIntrusive/ComboBoxLogic.h"

//#include "Alternative/Label.h"

#include "Silent/ComboBox.h"
#include "atomic_callback_store.hpp"

//#include "observable.hpp"

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

    /*
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
    */

    /*
    shared_obe<std::vector<std::string>> soItemList
    {
        {"a", "b", "c", "d"}
    };

    shared_obe<std::string> soItemSelected{"a"};

    auto * pComboBox = new QComboBox();
    pComboBox->show();

    NonIntrusive::ComboBoxLogic logic(pComboBox, soItemList, soItemSelected);
    */

    /*
    shared_obe<std::vector<std::string>> soItemList{{"a", "b", "c", "d"}};
    shared_obe<std::optional<std::size_t>> soItemSelection{2};

    auto * pComboBox = new QComboBox();
    pComboBox->show();

    ComboBoxLogic::List listLogic(pComboBox, soItemList);
    ComboBoxLogic::Selection selectionLogic(pComboBox, soItemSelection);
    ComboBoxLogic::Repopulate repopulateLogic(soItemList, soItemSelection);

    unique_txn{soItemList}.get() = {{}};

    std::future<void> f;

    auto changeItemList = [soItemList]() mutable
    {
        unique_txn{soItemList}.get() = {"x", "y"};
    };
    f = std::async(std::launch::async, changeItemList);

    auto changeItemListAgain = [soItemList]() mutable
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        unique_txn{soItemList}.get() = {"x", "y", "z"};
    };
    f = std::async(std::launch::async, changeItemListAgain);
    */

    /*
    auto * label = new QLabel();
    label->setText("string input:");

    auto * string = new GUI::Label();

    auto * layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(string);

    auto * widget = new QWidget();
    widget->setLayout(layout);
    widget->show();
    */

    Silent::ComboBox combo;
    combo.show();

    combo.insertItem(0, "a");
    combo.insertItem(1, "b");
    combo.addItem("c");
    combo.setCurrentIndex(std::nullopt);

    auto updateCurrentIndex = [&combo](std::optional<std::size_t> index)
    {
        combo.setCurrentIndex(index);
    };
    QObject::connect(&combo, &Silent::ComboBox::currentIndexChanged, updateCurrentIndex);

    return application.exec();
}
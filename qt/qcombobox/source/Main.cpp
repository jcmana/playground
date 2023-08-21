#include <iostream>
#include <thread>
#include <future>

#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    auto * pComboBox = new QComboBox();
    pComboBox->show();

    /*
    auto procedure = [pComboBox]
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        {
            auto update = [pComboBox]
            {
                pComboBox->addItem("a");
                pComboBox->addItem("b");
                pComboBox->addItem("c");
                pComboBox->addItem("d");
            };
            QMetaObject::invokeMethod(pComboBox, update);
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        {
            auto update = [pComboBox]
            {
                pComboBox->clear();
            };
            QMetaObject::invokeMethod(pComboBox, update);
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        {
            auto update = [pComboBox]
            {
                pComboBox->addItem("a");
                pComboBox->addItem("b");
                pComboBox->addItem("d");
                pComboBox->addItem("d");
            };
            QMetaObject::invokeMethod(pComboBox, update);
        }
    };
    auto future = std::async(std::launch::async, procedure);

    auto change = [](int index)
    {
        qDebug("current index changed = %d", index);
    };
    QObject::connect(pComboBox, &QComboBox::currentIndexChanged, change);
    */

    pComboBox->addItem("a");
    pComboBox->addItem("b");
    pComboBox->addItem("c");
    pComboBox->addItem("d");

    pComboBox->setPlaceholderText("x");
    pComboBox->setCurrentIndex(-1);
    pComboBox->setStyleSheet("

    return application.exec();
}

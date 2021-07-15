#include <thread>
#include <future>
#include <memory>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>

#include "Model.h"
#include "MainWidget.h"

enum class Approach
{
    PULL,
    PUSH,
    PUSHPULL,
};

constexpr static auto approach = Approach::PUSH;

int answer_computation(int question)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return question * 3 + 14;
}

void worker_procedure(const Model & question, Model & answer)
{
    answer.value = answer_computation(question.value);
    
    if (answer.callback)
    {
        answer.callback(answer.value);
    }

    if (answer.event)
    {
        answer.event();
    }
}

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Setup model:
    Model question;
    Model answer;

    // Setup worker actor:
    auto worker = std::thread();

    // Setup GUI actor:
    auto pMainWidget = new MainWidget();
    pMainWidget->show();

    // Setup timer actor:
    QTimer viewUpdateTimer;

    // Connect actors with model:
    QObject::connect(pMainWidget, &MainWidget::questionChanged, [&](int value)
    {
        question.value = value;
    });

    QObject::connect(pMainWidget, &MainWidget::answerRequested, [&]
    {
        if (worker.joinable())
        {
            worker.join();
        }

        worker = std::thread(std::bind(worker_procedure, std::cref(question), std::ref(answer)));
    });

    // How to update the computed answer in view?

    switch (approach)
    {
        case Approach::PULL:
        {
            QObject::connect(&viewUpdateTimer, &QTimer::timeout, pMainWidget, [&]
            {
                pMainWidget->setAnswer(answer.value);
            });

            viewUpdateTimer.setInterval(100);
            viewUpdateTimer.start();

            break;
        }

        case Approach::PUSH:
        {
            answer.callback = [&](int value)
            {
                QMetaObject::invokeMethod(pMainWidget, "setAnswer", Q_ARG(int, value));
            };
            break;
        }

        case Approach::PUSHPULL:
        {
            answer.event = [&]
            {
                const auto value = answer.value;
                QMetaObject::invokeMethod(pMainWidget, "setAnswer", Q_ARG(int, value));
            };
            break;
        }
    }

    // Run the application:
    const auto rc = application.exec();

    // Cleanup and exit:
    if (worker.joinable())
    {
        worker.join();
    }
    delete pMainWidget;

    return rc;
}
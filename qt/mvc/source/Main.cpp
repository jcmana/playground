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

constexpr static auto approach = Approach::PUSHPULL;

int answer_computation(int question)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return question * 3 + 14;
}

void worker_procedure_no_notification(int question, int & answer)
{
    answer = answer_computation(question);
}

void worker_procedure_with_callback(int question, int & answer, std::function<void(int)> & callback)
{
    answer = answer_computation(question);
    callback(answer);
}

void worker_procedure_with_event(int question, int & answer, std::function<void()> & event)
{
    answer = answer_computation(question);
    event();
}

void join(std::thread & t)
{
    if (t.joinable())
    {
        t.join();
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

        switch (approach)
        {
            case Approach::PULL:
            {
                worker = std::thread(std::bind(worker_procedure_no_notification, question.value, std::ref(answer.value)));
                break;
            }

            case Approach::PUSH:
            {
                worker = std::thread(std::bind(worker_procedure_with_callback, question.value, std::ref(answer.value), answer.callback));
                break;
            }

            case Approach::PUSHPULL:
            {
                worker = std::thread(std::bind(worker_procedure_with_event, question.value, std::ref(answer.value), answer.event));
                break;
            }
        }
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
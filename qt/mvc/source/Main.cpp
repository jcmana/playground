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

void task(Model & state, const Model & question, Model & answer, const bool & terminate)
{
    state.set(1);
    answer.set(0);

    auto q = question.value;
    auto a = 0;

    // compute the answer (q * 3 + 14) with the possibility to terminate early:

    for (auto n = 0; n < 3; ++n)
    {
        a += q;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (terminate)
        {
            state.set(-1);
            return;
        }
    }

    for (auto n = 0; n < 14; ++n)
    {
        a += 1;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (terminate)
        {
            state.set(-1);
            return;
        }
    }

    answer.set(a);
    state.set(0);
}

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Setup model
    Model question;
    Model answer;
    Model state;  // -1 = terminated; 0 = done; 1 = running

    // Worker task termination token
    auto terminate = false;
    
    // Setup worker actor
    auto worker = std::thread();

    // Setup GUI actor
    auto pMainWidget = new MainWidget();
    pMainWidget->show();

    // Setup timer actor
    QTimer viewUpdateTimer;

    // Connect actors with model:
    QObject::connect(pMainWidget, &MainWidget::questionChanged, [&](int value)
    {
        question.value = value;
    });

    QObject::connect(pMainWidget, &MainWidget::answerRequested, [&]
    {
        terminate = true;

        if (worker.joinable())
        {
            worker.join();
        }

        terminate = false;
        worker = std::thread(std::bind(task, std::ref(state), std::cref(question), std::ref(answer), std::cref(terminate)));
    });

    QObject::connect(pMainWidget, &MainWidget::terminateRequested, [&]
    {
        terminate = true;
    });

    // How to update the computed answer in view?

    switch (approach)
    {
        case Approach::PULL:
        {
            QObject::connect(&viewUpdateTimer, &QTimer::timeout, pMainWidget, [&]
            {
                pMainWidget->setAnswer(answer.value);
                
                if (state.value > 0)
                {
                    pMainWidget->setRunning();
                }
                else
                {
                    pMainWidget->setDone();
                }
            });

            viewUpdateTimer.setInterval(100);
            viewUpdateTimer.start();

            break;
        }

        case Approach::PUSH:
        {
            answer.callback = [&](int value)
            {
                auto proc = [pMainWidget, value]
                {
                    pMainWidget->setAnswer(value);
                };

                QMetaObject::invokeMethod(pMainWidget, proc);
            };

            state.callback = [&](int value)
            {
                auto proc = [pMainWidget, value]
                {
                    if (value > 0)
                    {
                        pMainWidget->setRunning();
                    }
                    else
                    {
                        pMainWidget->setDone();
                    }
                };

                QMetaObject::invokeMethod(pMainWidget, proc);
            };

            break;
        }

        case Approach::PUSHPULL:
        {
            answer.event = [&]
            {
                const auto value = answer.value;

                auto proc = [pMainWidget, value]
                {
                    pMainWidget->setAnswer(value);
                };

                QMetaObject::invokeMethod(pMainWidget, proc);
            };

            state.event = [&]
            {
                const auto value = state.value;

                auto proc = [pMainWidget, value]
                {
                    if (value > 0)
                    {
                        pMainWidget->setRunning();
                    }
                    else
                    {
                        pMainWidget->setDone();
                    }
                };

                QMetaObject::invokeMethod(pMainWidget, proc);
            };

            break;
        }
    }

    state.set(0);

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
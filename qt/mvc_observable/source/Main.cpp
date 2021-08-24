#include <QtCore/QObject>
#include <QtWidgets/QApplication>

#include "shared_observable.hpp"
#include "connect.hpp"

#include "State.h"
#include "Task.h"
#include "MainWidget.h"

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Setup observables:
    shared_observable<State> state;
    shared_observable<bool> running;

    shared_observable<int> question;
    shared_observable<int> answer;

    shared_observable<bool> compute;
    shared_observable<bool> terminate;
    
    // Setup worker actor
    auto worker = std::thread();

    // Setup GUI actor
    auto pMainWidget = new MainWidget();
    pMainWidget->show();

    // Connect observables:
    compute.observe([&](bool value)
    {
        if (value)
        {
            unique_txn{terminate} = true;
            if (worker.joinable())
            {
                worker.join();
            }
            unique_txn{terminate} = false;

            worker = std::thread(task, state, question, answer, terminate);
        }
    });

    state.observe([&](State state)
    {
        switch (state)
        {
            case State::IDLE:
            case State::FINISHED:
            case State::TERMINATED:
                unique_txn{running} = false;
                break;

            case State::RUNNING:
                unique_txn{running} = true;
                break;
        }
    });

    connect(running, pMainWidget, &MainWidget::setCompuationRunning);
    connect(answer, pMainWidget, &MainWidget::setAnswer);

    connect(pMainWidget, &MainWidget::buttonClicked, compute);
    connect(pMainWidget, &MainWidget::buttonTerminateClicked, terminate);
    connect(pMainWidget, &MainWidget::questionChanged, question);

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
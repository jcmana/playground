#include <thread>
#include <future>
#include <memory>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>

#include "shared_observable.hpp"

#include "State.h"
#include "MainWidget.h"

void task(shared_observable<State> state, shared_observable<int> question, shared_observable<int> answer, shared_observable<bool> terminate)
{
    // reset the output states:
    unique_txn{state} = State::RUNNING;
    unique_txn{answer} = 0;

    // compute the answer (q * 3 + 14) with the possibility to terminate early:
    auto q = shared_txn{question}.get<0>();
    auto a = 0;

    for (auto n = 0; n < 3; ++n)
    {
        a += q;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (shared_txn{terminate}.get<0>())
        {
            unique_txn{state} = State::TERMINATED;
            return;
        }
    }

    for (auto n = 0; n < 14; ++n)
    {
        a += 1;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (shared_txn{terminate}.get<0>())
        {
            unique_txn{state} = State::TERMINATED;
            return;
        }
    }

    // set the output states:
    unique_txn{answer} = a;
    unique_txn{state} = State::FINISHED;
}

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Setup observables:
    shared_observable<State> state;
    shared_observable<int> question;
    shared_observable<int> answer;

    shared_observable<bool> compute;
    shared_observable<bool> terminate;
    
    // Setup worker actor
    auto worker = std::thread();

    // Setup GUI actor
    auto pMainWidget = new MainWidget(state, question, answer, compute, terminate);
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
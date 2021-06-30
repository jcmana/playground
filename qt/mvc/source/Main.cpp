#include <thread>
#include <future>
#include <memory>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpacerItem>

void worker_procedure(int question, int & answer, std::function<void(int)> & callback, std::function<void()> & event)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    answer = question * 3 + 14;

    if (callback)
    {
        callback(answer);
    }

    if (event)
    {
        event();
    }
}

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);

    // Setup model:
    auto question = 0;
    auto answer = 0;
    std::function<void(int)> answerCallback;
    std::function<void()> answerEvent;

    // Setup worker actor:
    auto worker = std::thread();

    // Setup view actor:
    auto pMainWidget = new QWidget();
    pMainWidget->show();

    auto pLayout = new QGridLayout(pMainWidget);
    
    auto pSpinBox = new QSpinBox();
    pSpinBox->setMinimum(0);
    pSpinBox->setMaximum(100);
    pSpinBox->setValue(question);

    auto pPushButton = new QPushButton();
    pPushButton->setText("Compute the answer to universe");

    auto pSpinBoxResult = new QSpinBox();
    pSpinBoxResult->setMinimum(0);
    pSpinBoxResult->setMaximum(100'000);
    pSpinBoxResult->setReadOnly(true);
    pSpinBoxResult->setValue(answer);

    auto pSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

    pMainWidget->setLayout(pLayout);
    pLayout->addWidget(pSpinBox, 0, 0);
    pLayout->addWidget(pPushButton, 0, 1);
    pLayout->addWidget(pSpinBoxResult, 1, 0, 1, 2);
    pLayout->addItem(pSpacer, 2, 0, 1, 2);

    QTimer viewUpdateTimer;
    viewUpdateTimer.setInterval(100);

    // Connect:
    QObject::connect(pSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value)
    {
        question = value;
    });

    QObject::connect(pPushButton, &QPushButton::clicked, [&]
    {
        if (worker.joinable())
        {
            worker.join();
        }
        worker = std::thread(std::bind(worker_procedure, question, std::ref(answer), std::ref(answerCallback), std::ref(answerEvent)));
    });

    // How to update the computed answer in view?

    // #1 - Pull approach (value polling)
    if (false)
    {
        QObject::connect(&viewUpdateTimer, &QTimer::timeout, pSpinBoxResult, [&]
        {
            pSpinBoxResult->setValue(answer);
        });
    }

    // #2 - Push approach (callbacks)
    if (false)
    {
        answerCallback = [&](int value)
        {
            QMetaObject::invokeMethod(pSpinBoxResult, "setValue", Q_ARG(int, value));
        };
    }

    // #3 - Push+pull approach (event)
    if (true)
    {
        answerEvent = [&]
        {
            // First, retrieve the value from source model
            const auto value = answer;
            // Second, update the view
            QMetaObject::invokeMethod(pSpinBoxResult, "setValue", Q_ARG(int, value));
        };
    }

    // Run the application:
    viewUpdateTimer.start();
    const auto rc = application.exec();

    // Cleanup and exit:
    if (worker.joinable())
    {
        worker.join();
    }
    delete pMainWidget;

    return rc;
}
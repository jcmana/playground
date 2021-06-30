#include <thread>
#include <future>
#include <memory>
#include <functional>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpacerItem>

void worker_procedure(int question, int & answer)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    answer = question * 3 + 14;

    // Answer is now computed
}

int main(int argc, char * argv[])
{
    QApplication application(argc, argv);
    QWidget mainWidget;

    // Setup model:
    auto question = 0;
    auto answer = 0;

    // Setup worker actor:
    auto worker = std::thread();

    // Setup view actor:
    auto pLayout = new QGridLayout(&mainWidget);
    
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

    mainWidget.setLayout(pLayout);
    pLayout->addWidget(pSpinBox, 0, 0);
    pLayout->addWidget(pPushButton, 0, 1);
    pLayout->addWidget(pSpinBoxResult, 1, 0, 1, 2);
    pLayout->addItem(pSpacer, 2, 0, 1, 2);

    // Connect:
    QObject::connect(pSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value)
    {
        question = value;
    });

    QObject::connect(pPushButton, &QPushButton::clicked, [&]
    {
        // Event-like situation, upon user request start the computation
        worker = std::thread(std::bind(worker_procedure, question, std::ref(answer)));
    });

    // How to update the computed answer in view?

    // Run the application:
    mainWidget.show();
    const auto rc = application.exec();

    // Cleanup:
    if (worker.joinable())
    {
        worker.join();
    }

    return rc;
}
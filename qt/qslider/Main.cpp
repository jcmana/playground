#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

int main(int argc, char ** argv)
{
    QApplication application(argc, argv);

    QWidget main;
    main.show();

    auto * layout = new QVBoxLayout(&main);
    main.setLayout(layout);

    auto * slider = new QSlider();
    layout->addWidget(slider);
    slider->setStyleSheet("QSlider::add-page:vertical { background: white; }");



    return application.exec();
}
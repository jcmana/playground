#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGridLayout>

#include "Slider.h"

int main(int argc, char ** argv)
{
    QApplication application(argc, argv);
    application.setStyle(QStyleFactory::create("Fusion"));

    QWidget main;
    main.show();

    auto * layout = new QGridLayout(&main);
    main.setLayout(layout);

    auto css = 
        "QSlider::groove:horizontal {"
        "border:1px solid #bbb;"
        "background:rgba(0, 0, 0, 0);"
        "height:4px;"
        "border-radius:2px;"
        "}"
        "QSlider::handle:horizontal {"
        "background:rgba(220, 220, 220);"
        "border:1px solid rgb(128, 128, 128);"
        "border-radius: 2px;"
        "width:16px;"
        "margin:-2px 10;"
        "};";

    auto cssTescan = 
        "QSlider::groove:horizontal {"
        "border-color: #444444;"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
        "}"
        "QSlider::handle:horizontal {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
        "border-color: #5c5c5c;"
        "}"
        "QSlider::handle:horizontal:!enabled {"
        "background: #838383;"
        "}"
        "QSlider::add-page:horizontal {"
        "background: white;"
        "}"
        "QSlider::sub-page:horizontal {"
        "background: #1e9ddc;"
        "}"
        "QSlider::add-page:horizontal:!enabled {"
        "background: #838383;"
        "}"
        "QSlider::sub-page:horizontal:!enabled {"
        "background: #838383;"
        "}";

    auto * sliderV = new Tescan::EBL::VerticalSlider();
    sliderV->setValue(50);
    //sliderV->setStyleSheet("QSlider::groove:vertical { background: rgba(50, 50, 50); }");
    //sliderV->setStyleSheet(css);
    layout->addWidget(sliderV, 0, 0);

    auto * sliderH = new Tescan::EBL::HorizontalSlider();
    sliderH->setValue(50);
    //sliderH->setOrientation(Qt::Orientation::Horizontal);
    //sliderH->setStyleSheet("QSlider::sub-page:horizontal{ background: white; }");
    //sliderH->setStyleSheet(css);
    layout->addWidget(sliderH, 0, 1);

    return application.exec();
}
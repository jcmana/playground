#include <QtCore/QString>
#include <QtCore/QFile>

#include "FramesTest.h"

FramesTest::FramesTest(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QFile stylesheetFile(":/FramesTest/Style.qss");
    stylesheetFile.open(QFile::ReadOnly);
    QString stylesheet(stylesheetFile.readAll());
    setStyleSheet(stylesheet);

    /*
    ui.frame->setBorderColor(FlatFrame::Right, Qt::red);
    ui.frame->setBorderSize(FlatFrame::Right, 1);
    ui.frame->setBorderStyle(FlatFrame::Right, FlatFrame::Solid);

    ui.frame_2->setBorderColor(FlatFrame::Bottom, Qt::red);
    ui.frame_2->setBorderSize(FlatFrame::Bottom, 1);
    ui.frame_2->setBorderStyle(FlatFrame::Bottom, FlatFrame::Solid);
    */

    //setWindowFlag(Qt::FramelessWindowHint);
}

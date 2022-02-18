#pragma once

#include <QtWidgets/QWidget>

#include "ui_FramesTest.h"

class FramesTest : 
    public QWidget
{
    Q_OBJECT

public:
    FramesTest(QWidget *parent = Q_NULLPTR);

private:
    Ui::FramesTestClass ui;
};

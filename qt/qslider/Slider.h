#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>

namespace Tescan::EBL
{

class HorizontalSlider : 
    public QSlider
{
public:
    HorizontalSlider(QWidget * parent = nullptr);

    virtual void mouseMoveEvent(QMouseEvent * e) override;
    virtual void mousePressEvent(QMouseEvent * e) override;

private:
    int positionToValue(int x);
};

class VerticalSlider : 
    public QSlider
{
public:
    VerticalSlider(QWidget * parent = nullptr);

public:
    virtual void mouseMoveEvent(QMouseEvent * e) override;
    virtual void mousePressEvent(QMouseEvent * e) override;

private:
    int positionToValue(int y);
};

} // namespace Tescan::EBL

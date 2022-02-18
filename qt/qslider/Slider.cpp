#include <QtGui/QMouseEvent>

#include "Slider.h"

namespace Tescan::EBL
{

constexpr static auto SLIDER_H_STYLESHEET =
    "QSlider::groove:horizontal {"
        "border:1px solid #bbb;"
        "border-radius:2px;"
        "background:rgba(0, 0, 0, 0);"
        "height:4px;"
    "}"

    "QSlider::handle:horizontal {"
        "background:rgba(220, 220, 220);"
        "border:1px solid rgb(128, 128, 128);"
        "border-radius: 2px;"
        "width:10px;"
        "margin:-4px 0;"
    "};";


HorizontalSlider::HorizontalSlider(QWidget * parent) :
    QSlider(parent)
{
    setOrientation(Qt::Orientation::Horizontal);
    setStyleSheet(SLIDER_H_STYLESHEET);
}

void HorizontalSlider::mouseMoveEvent(QMouseEvent * e)
{
    setValue(positionToValue(e->x()));
}

void HorizontalSlider::mousePressEvent(QMouseEvent * e)
{
    setValue(positionToValue(e->x()));
}

int HorizontalSlider::positionToValue(int x)
{
    const auto valueRange = maximum() - minimum();
    const auto widthRange = width();
    const auto ratio = double(x) / double(widthRange);
    const auto value = int(valueRange * ratio + 0.5);

    return value;
}

constexpr static auto SLIDER_V_STYLESHEET =
    "QSlider::groove:vertical {"
        "border:1px solid #bbb;"
        "border-radius:2px;"
        "background:rgba(0, 0, 0, 0);"
        "width:4px;"
    "}"

    "QSlider::handle:vertical {"
        "background:rgba(220, 220, 220);"
        "border:1px solid rgb(128, 128, 128);"
        "border-radius: 2px;"
        "height:10px;"
        "margin:0px -4;"
    "};";

VerticalSlider::VerticalSlider(QWidget * parent) :
    QSlider(parent)
{
    setOrientation(Qt::Orientation::Vertical);
    setStyleSheet(SLIDER_V_STYLESHEET);
}

void VerticalSlider::mouseMoveEvent(QMouseEvent * e)
{
    setValue(positionToValue(e->y()));
}

void VerticalSlider::mousePressEvent(QMouseEvent * e)
{
    setValue(positionToValue(e->y()));
}

int VerticalSlider::positionToValue(int y)
{
    const auto valueRange = maximum() - minimum();
    const auto heightRange = height();
    const auto ratio = double(y) / double(heightRange);
    const auto value = maximum() - int(valueRange * ratio + 0.5);

    return value;
}


} // namespace Tescan::EBL

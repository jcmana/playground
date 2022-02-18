#include <QtWidgets/QLayout>

#include "FlatFrame.h"

FlatFrame::FlatFrame(QWidget * parent) :
    QFrame(parent)
{
    setFrameShadow(QFrame::Plain);
    setFrameStyle(QFrame::NoFrame);
}

void FlatFrame::setBorderColor(BorderSide side, QColor color)
{
    m_borders[side].color = color;
    update();
}

void FlatFrame::setBorderSize(BorderSide side, int size)
{
    m_borders[side].size = size;
    update();
}

void FlatFrame::setBorderStyle(BorderSide side, BorderStyle style)
{
    m_borders[side].style = style;
    update();
}

void FlatFrame::update()
{
    QString styleSheet = "#" + objectName() + "\n";
    styleSheet += "{\n";

    for (auto side : {Left, Right, Top, Bottom})
    {
        styleSheet += "    border-" + borderSideToString(side) + "-style:" + borderStyleToString(m_borders[side].style) + ";\n";
        styleSheet += "    border-" + borderSideToString(side) + "-width:" + QString::number(m_borders[side].size) + ";\n";
        styleSheet += "    border-" + borderSideToString(side) + "-color:" + m_borders[side].color.name() + ";\n";
    }

    styleSheet += "}\n";

    setStyleSheet(styleSheet);
}

QString FlatFrame::borderSideToString(BorderSide side)
{
    switch (side)
    {
        case Left:
            return "left";
        case Right:
            return "right";
        case Top:
            return "top";
        case Bottom:
            return "bottom";
    }

    throw "Unknown border side";
}

QString FlatFrame::borderStyleToString(BorderStyle style)
{
    switch (style)
    {
        case None:
            return "none";
        case Solid:
            return "solid";
        case Dashed:
            return "dashed";
        case Dotted:
            return "dotted";
    }

    throw "Unknown border style";
}

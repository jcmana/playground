#pragma once

#include <array>

#include <QtWidgets/QFrame>

class FlatFrame :
    public QFrame
{
    Q_OBJECT

public:
    enum BorderSide
    {
        Left,
        Right,
        Top,
        Bottom
    };

    enum BorderStyle
    {
        None,
        Solid,
        Dashed,
        Dotted
    };

public:
    FlatFrame(QWidget  * parent = nullptr);

public slots:
    void setBorderColor(BorderSide side, QColor color);
    void setBorderSize(BorderSide side, int size);
    void setBorderStyle(BorderSide side, BorderStyle style);

private:
    struct Border
    {
        QColor color = Qt::black;
        int size = 0;
        BorderStyle style = None;
    };

private:
    void update();

    QString borderSideToString(BorderSide side);
    QString borderStyleToString(BorderStyle style);

private:
    std::array<Border, sizeof(BorderSide)> m_borders;
};

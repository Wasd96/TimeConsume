#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimerEvent>
#include <QPainter>
#include <QMouseEvent>

#include "timecore.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void timerEvent(QTimerEvent *ev);

    void paintEvent(QPaintEvent *ev);

    void mousePressEvent(QMouseEvent *ev);

    void resizeEvent(QResizeEvent *ev);

private:
    Ui::Widget *ui;

    TimeCore core;
};

#endif // WIDGET_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimerEvent>
#include <QPainter>

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

private:
    Ui::Widget *ui;

    TimeCore core;
};

#endif // WIDGET_H

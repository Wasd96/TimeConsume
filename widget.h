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

    QColor getApprox(QImage *img, int x, int y, int w, int h);
    void saveTofile();

    void timerEvent(QTimerEvent *ev);

    void paintEvent(QPaintEvent *ev);

    void mousePressEvent(QMouseEvent *ev);

    void resizeEvent(QResizeEvent *ev);

private slots:
    void on_stats_clicked();

    void on_clear_clicked();

private:
    Ui::Widget *ui;

    TimeCore core;

    int mainLoop;
    int afkLoop;

    QImage afkOld;              // старый образ активного окна
    QPoint curOld;              // старое положение мыши

    bool statShowing;           // показ статистики
};

#endif // WIDGET_H

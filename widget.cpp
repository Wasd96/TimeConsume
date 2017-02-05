#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    startTimer(1000);               // считывание каждую секунду
}

Widget::~Widget()
{
    delete ui;
}


void Widget::timerEvent(QTimerEvent *ev)
{

    core.update();

    foreach (TimeUnit TU, core.units)
    {
        qDebug() << TU.processName;
        for (int i = 0; i < TU.windowNames.size(); i++)
        {
            qDebug() << TU.windowUse.at(i) << "   " << TU.windowNames.at(i);
        }
        qDebug() << TU.allUse;
    }
    qDebug() << "   ";


}

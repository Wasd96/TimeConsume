#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    srand((uint)ui);
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

    repaint();
}

void Widget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    p.drawRect(230,10,510,400);
    QFont font;
    font.setPixelSize(25);
    p.setFont(font);

    int unitsSize = core.units.size();
    if (unitsSize > 0)
    {
        int* sortArr = new int[unitsSize];
        for (int i = 0; i < unitsSize; i++)
        {
            sortArr[i] = i;
        }

        for (int i = 0; i < unitsSize; i++)
        {
            bool flag = false;
            for (int j = 0; j < unitsSize-i-1; j++)
            {
                if (core.units.at(sortArr[j]).allUse <
                        core.units.at(sortArr[j+1]).allUse)
                {
                    int temp = sortArr[j+1];
                    sortArr[j+1] = sortArr[j];
                    sortArr[j] = temp;
                    flag = true;
                }
            }
            if (!flag) break;
        }

        for (int i = 0; i < unitsSize; i++)
        {
            p.fillRect(235,
                       20+30*sortArr[i],
                       500*((float)core.units.at(i).allUse/(float)core.secActive),
                       25,
                       core.units.at(i).color);

            p.drawText(245,
                       40+30*sortArr[i],
                       core.units.at(i).processName);
        }

        float graphWidth = 510.0/(float)core.secActive;
        for (int i = 0; i < core.secActive; i++)
        {
            p.fillRect(230+graphWidth*i, 420, graphWidth+1, 170,
                       core.units.at(core.consumes[i]).color);
        }

        delete[] sortArr;
    }

}

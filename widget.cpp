#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    srand((uint)&core);
    startTimer(1000);               // считывание каждую секунду
}

Widget::~Widget()
{
    delete ui;
}


void Widget::timerEvent(QTimerEvent *ev)
{
    core.update();

    repaint();
}

void Widget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    p.drawRect(250,10,width()-260,height()-120);
    p.drawRect(250,10,width()-260,height()-100);
    QFont font;
    font.setPixelSize(25);
    p.setFont(font);

    int unitsSize = core.units.size();
    if (unitsSize > 0)
    {
        if (core.selected != -1)
        {
            p.fillRect(250, 15+30*core.selected, width()-260, 34, Qt::lightGray);
            p.drawRect(250, 15+30*core.selected, width()-260, 34);
        }


        for (int i = 0; i < unitsSize && 20+30*i < height()-120; i++)
        {
            p.fillRect(255,
                       20+30*i,
                       (width()-270)*((float)core.units.at(i).allUse/(float)core.secActive),
                       25,
                       core.units.at(i).color);

            p.drawText(265,
                       40+30*i,
                       core.units.at(i).processName);
        }

        font.setPixelSize(16);
        p.setFont(font);
        for (int i = 0; i < unitsSize && 20+30*i < height()-120; i++)
        {
            p.drawText(width()-70,
                       40+30*i,
                       QString::number((int)((float)core.units.at(i).allUse/
                                             (float)core.secActive*100))+"%");

            QString str = core.units[i].GetTime(core.units[i].allUse);
            str += "  -";
            p.drawText(QRect(width()-150,24+30*i,70,20), Qt::AlignRight, str);
        }


        float graphWidth = ((float)(width()-20))/(float)core.secActive;
        for (int i = 0; i < core.secActive; i++)
        {
            if (core.selected != -1)
            {
                if (core.consumes[i] == core.selected)
                {
                    p.fillRect(10+graphWidth*i, height()-80, graphWidth+1, 70,
                               Qt::green);
                }
                else
                {
                    p.fillRect(10+graphWidth*i, height()-80, graphWidth+1, 70,
                               Qt::darkRed);
                }
            }
            else
            {
                p.fillRect(10+graphWidth*i, height()-80, graphWidth+1, 70,
                         core.units.at(core.consumes[i]).color);
            }
        }

    }


    ui->windows->clear();
    if (core.selected != -1)
    {
        QString str;
        for (int i = 0; i < core.units.at(core.selected).windowNames.size(); i++)
        {
            str = core.units[core.selected].ToString(i);
            ui->windows->addItem(str);
        }
        font.setPixelSize(14);
        p.setFont(font);
        p.drawText(250, height()-95, core.units.at(core.selected).fullName);
    }

}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->x() > 230 && ev->y() > 15
            && ev->y() < (15+core.units.size()*30))
    {
        core.selected = (ev->y() - 15)/30;
    }
    else
    {
        if (ev->x() > 10 && ev->x() < width()-10
                && ev->y() > height()-80 && ev->y() < height()-10
                && core.selected == -1)
        {
            QPixmap px = this->grab(QRect(10, height()-80, width()-10, 70));
            QImage img = px.toImage();
            QColor color = img.pixelColor(ev->x() - 10, ev->y() - (height()-80));

            for (int i = 0; i < core.units.size(); i++)
            {
                if (core.units.at(i).color == color)
                {
                    core.selected = i;
                }
            }
        }
        else
        {
            core.selected = -1;
        }
    }

    repaint();
}

void Widget::resizeEvent(QResizeEvent *ev)
{
    ui->windows->resize(ui->windows->width(), height()-248);
}

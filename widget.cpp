#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QScreen>
#include <QTime>
#include <QFile>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    srand((uint)&core);
    mainLoop = startTimer(1000);    // считывание каждую секунду
    afkLoop = startTimer(60000);    // проверка на афк раз в минуту
    statShowing = false;

    setWindowTitle("Time Consumption");
}

Widget::~Widget()
{
    saveTofile(); // сохранение результатов при корректном завершении работы
    delete ui;
}

// получение среднего цвета в выбранной области картинки (для афк)
QColor Widget::getApprox(QImage *img, int x, int y, int w, int h)
{
    QColor color;
    color.setRgb(0,0,0);
    ulong r = 0;
    ulong g = 0;
    ulong b = 0;
    for (int i = x; i < x+w; i++)
    {
        for (int j = y; j < y+h; j++)
        {
            r += img->pixelColor(i,j).red()/3;
            g += img->pixelColor(i,j).green()/3;
            b += img->pixelColor(i,j).blue()/3;
        }
    }
    r = r/(w*h)*3;
    g = g/(w*h)*3;
    b = b/(w*h)*3;

    color.setRgb(r,g,b);
    return color;
}

void Widget::saveTofile() // сохранение статистики в файл
{
    QString filename = "stats.log";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&file);
        out << QDate::currentDate().toString("dd.MM.yyyy")+"_"+
               QTime::currentTime().toString("hh.mm.ss")+"\n";
        for (int i = 0; i < core.units.size(); i++)
        {
            if (core.units.at(i).fullName.isEmpty())
                out << "AllAbsorbingEmptiness";
            out << core.units.at(i).processName;
            out << '\n';
            out << core.units.at(i).allUse;
            out << '\n';
        }
        out << '\n';
    }

    file.close();
}


void Widget::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == mainLoop && core.afk != true) // добавление
    {
        core.update();
        repaint();
    }

    if (ev->timerId() == afkLoop) // цикл для афк
    {
        QCursor cur;
        if (cur.pos() != curOld) // мышь двинули - не афк
        {
            core.afk = false;
            killTimer(afkLoop);
            afkLoop = startTimer(60000);
            curOld = cur.pos();
            core.ensure();
            return;
        }

        QScreen *screen = QGuiApplication::primaryScreen();
        WId winda = (WId)GetForegroundWindow();
        QPixmap px = screen->grabWindow(winda); // получение образа активного окна
        QImage img = px.toImage();

        if (img.size() == afkOld.size()) // разные экраны - не афк
        {

            int w = img.size().width()/5;
            int h = img.size().height()/4;
            QColor colOld;
            QColor colNew;
            int dr = 0;
            int dg = 0;
            int db = 0;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    colOld = getApprox(&afkOld, i*w, j*h, w, h);
                    colNew = getApprox(&img, i*w, j*h, w, h);

                    dr += abs(colNew.red() - colOld.red());
                    dg += abs(colNew.green() - colOld.green());
                    db += abs(colNew.blue() - colOld.blue());
                }
            }
            int totalD = dr + dg + db; // общая "разница" в изображениях
            if (totalD < 50) // меньше 50 - афк
            {
                killTimer(afkLoop);
                afkLoop = startTimer(5000); // в афк скан чаще
                core.afk = true;
                core.rollBack();
                repaint();
            }
            else
            {
                core.afk = false;
                killTimer(afkLoop);
                afkLoop = startTimer(60000);
                core.ensure();
            }
        }
        else
        {
            core.ensure();
        }
        afkOld = img; // сохранение нового образа активного окна
        curOld = cur.pos(); // позиции курсора
    }
}

void Widget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    p.drawRect(250,10,width()-260,height()-120); // рамки для процессов
    p.drawRect(250,10,width()-260,height()-100);
    QFont font;
    font.setPixelSize(25);
    p.setFont(font);

    int wdt = width();
    int hgt = height();

    int unitsSize = 0;
    if (statShowing) // если показ статистики, то одно
        unitsSize = core.statNames.size();
    else             // если нет, то другое
        unitsSize = core.units.size();
    if (unitsSize > 0)
    {
        if (core.selected != -1)
        {
            p.fillRect(250, 15+30*core.selected, wdt-260, 34, Qt::lightGray);
            p.drawRect(250, 15+30*core.selected, wdt-260, 34);
        }

        double coef = 0; // длина полосы
        QString str;
        QColor color;
        for (int i = 0; i < unitsSize && 20+30*i < hgt-120; i++)
        {

            if (statShowing)
            {
                coef = (float)core.statUses.at(i)/(float)core.statAllUse;
                str = core.statNames.at(i);
                color = Qt::lightGray;
            }
            else
            {
                coef = (float)core.units.at(i).allUse/(float)core.secActive;
                str = core.units.at(i).processName;
                color = core.units.at(i).color;
            }

            p.fillRect(255, 20+30*i, (wdt-270)*coef, 25, color);
            p.drawText(265, 40+30*i, str);
        }

        font.setPixelSize(16);
        p.setFont(font);
        for (int i = 0; i < unitsSize && 20+30*i < hgt-120; i++)
        {
            if (statShowing)
            {
                coef = (float)core.statUses.at(i)/(float)core.statAllUse;
                str = TimeUnit::GetTime(core.statUses.at(i));
                str += "  -";
            }
            else
            {
                coef = (float)core.units.at(i).allUse/(float)core.secActive;
                str = TimeUnit::GetTime(core.units.at(i).allUse);
                str += "  -";
            }

            p.drawText(wdt-70, 40+30*i, QString::number((int)(coef*100))+"%");
            p.drawText(QRect(wdt-150,24+30*i,70,20), Qt::AlignRight, str);
        }

        if (statShowing)
        {
            p.fillRect(10, hgt-80, wdt-20, 70, Qt::gray);
            font.setPixelSize(25);
            p.setFont(font);
            p.drawText(QRect(10, hgt-80, wdt-20, 70),
                       Qt::AlignHCenter | Qt::AlignVCenter,
                       "Нажмите где угодно для возврата");
        }
        else // вывод шкалы использования
        {
            float graphWidth = ((float)(wdt-20))/(float)core.secActive;
            for (int i = 0; i < core.secActive; i++)
            {
                if (core.selected != -1)
                {
                    if (core.consumes[i] == core.selected)
                    {
                        p.fillRect(10+graphWidth*i, hgt-80, graphWidth+1, 70,
                                   Qt::green);
                    }
                    else
                    {
                        p.fillRect(10+graphWidth*i, hgt-80, graphWidth+1, 70,
                                   Qt::darkRed);
                    }
                }
                else
                {
                    p.fillRect(10+graphWidth*i, hgt-80, graphWidth+1, 70,
                             core.units.at(core.consumes[i]).color);
                }
            }
        }

    }


    ui->windows->clear();
    if (core.selected != -1) // если выбран процесс, то вывод его окон
    {
        QString str;
        int size = core.units.at(core.selected).windowNames.size();
        for (int i = 0; i < size; i++)
        {
            str = core.units[core.selected].ToString(i);
            ui->windows->addItem(str);
        }
        font.setPixelSize(14);
        p.setFont(font);
        p.drawText(250, hgt-95, core.units.at(core.selected).fullName);
    }

}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    int wdt = width();
    int hgt = height();

    if (ev->x() > 230 && ev->y() > 15
            && ev->y() < (15+core.units.size()*30))
    {
        core.selected = (ev->y() - 15)/30; // выбрали процесс
    }
    else
    {
        if (ev->x() > 10 && ev->x() < wdt-10
                && ev->y() > hgt-80 && ev->y() < hgt-10
                && core.selected == -1)
        {
            QPixmap px = this->grab(QRect(10, hgt-80, wdt-10, 70));
            QImage img = px.toImage();
            QColor color = img.pixelColor(ev->x() - 10, ev->y() - (hgt-80));

            for (int i = 0; i < core.units.size(); i++)
            {
                if (core.units.at(i).color == color)
                {
                    core.selected = i; // процесс выбирается из шкалы по цвету
                }
            }
        }
        else
        {
            core.selected = -1;
        }
    }

    statShowing = false;

    repaint();
}

void Widget::resizeEvent(QResizeEvent *ev)
{
    ui->windows->resize(ui->windows->width(), height()-248);
}

void Widget::on_stats_clicked() // показ статистики
{
    core.statNames.clear();
    core.statUses.clear();
    core.statAllUse = 0;
    QString filename = "stats.log";
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QString str;
        QTextStream in(&file);

        while(!in.atEnd())
        {
            str = in.readLine();
            if (str.size() > 1)
            {
                if (str.at(0).isDigit() && str.size() >= 18) // дата
                    continue;

                if (!str.at(0).isDigit()) // это не число (процесс)
                {
                    bool existWin = false;
                    int size = core.statNames.size();
                    for (int i = 0; i < size; i++)
                    {
                        if (core.statNames.at(i) == str)
                        {
                            existWin = true;
                            str = in.readLine();
                            core.statUses[i] = core.statUses[i] + str.toLong();
                            core.statAllUse += str.toLong();
                            break;
                        }
                    }
                    if (!existWin)
                    {
                        core.statNames.append(str);
                        str = in.readLine();

                        core.statUses.append(str.toLong());
                        core.statAllUse += str.toLong();
                    }
                }
            }
        }

    }

    file.close();

    int pos = core.statNames.indexOf("AllAbsorbingEmptiness");
    if (pos >= 0)
        core.statNames[pos] = " "; // исправление обратно

    statShowing = true;
    core.selected = -1;
}

void Widget::on_clear_clicked() // удаление сохранений
{
    statShowing = false;
    core.selected = -1;
    core.statNames.clear();
    core.statUses.clear();
    core.statAllUse = 0;

    QFile file("stats.log");
    file.remove();
}

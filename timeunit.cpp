#include "timeunit.h"

TimeUnit::TimeUnit()
{

}

TimeUnit::TimeUnit(QString fullName, QString windowName)
{
    this->fullName = fullName;
    processName = fullName.right(fullName.size()
                                 - fullName.lastIndexOf('\\')
                                 - 1);
    processName.chop(4);
    windowNames.append(windowName);
    windowUse.append(1);
    windowUseTemp.append(0);
    allUse = 1;

    color = QColor(50+rand()%150, 50+rand()%150, 50+rand()%150);
}

void TimeUnit::RollBack() // откат
{
    for (int i = 0; i < windowUse.size(); i++)
    {
        if (windowUse[i] <= windowUseTemp[i])
            windowUse[i] = 0;
        else
            windowUse[i] -= windowUseTemp[i];
    }
    allUse -= allUseTemp;

    Ensure();
    Sort();
}

void TimeUnit::Ensure() // закрепление (возможный откат обнуляется)
{
    for (int i = 0; i < windowUseTemp.size(); i++)
    {
        windowUseTemp[i] = 0;
    }
    allUseTemp = 0;
}

void TimeUnit::Sort() // сортировка окон
{
    int size = windowNames.size();
    int temp;
    QString tempstr;
    for (int i = 0; i < size; i++)
    {
        for (int j = size-1; j > i; j--)
        {
            if (windowUse.at(j) > windowUse.at(j-1))
            {
                temp = windowUse.at(j);
                windowUse[j] = windowUse.at(j-1);
                windowUse[j-1] = temp;
                temp = windowUseTemp.at(j);
                windowUseTemp[j] = windowUseTemp.at(j-1);
                windowUseTemp[j-1] = temp;
                tempstr = windowNames.at(j);
                windowNames[j] = windowNames.at(j-1);
                windowNames[j-1] = tempstr;
            }
        }
    }
}

void TimeUnit::AddUsage(QString windowName)
{
    int pos = windowNames.indexOf(windowName);
    if (pos > -1) // если такое окно есть, то увеличиваем
    {
        windowUse[pos] +=  1;
        windowUseTemp[pos] += 1;
    }
    else // иначе добавляем
    {
        windowNames.append(windowName);
        windowUse.append(1);
        windowUseTemp.append(1);
    }

    allUse = allUse + 1;
    allUseTemp = allUseTemp + 1;

    Sort();
}

QString TimeUnit::ToString(int pos)
{
    QString resultStr = "";
    resultStr += GetTime(windowUse.at(pos));
    resultStr += " - ";
    resultStr += QString::number((int)((float)windowUse.at(pos)/(float)allUse*100.0)) + "%";
    resultStr += " - ";
    resultStr += windowNames.at(pos);

    return resultStr;
}

QString TimeUnit::GetTime(int time)
{
    QString resultStr = "";
    if (time < 3600)
    {
        resultStr += QString::number(time/60);
        resultStr += ":";
        if (time%60 < 10)
            resultStr += "0";
        resultStr += QString::number(time%60);
    }
    else
    {
        resultStr += QString::number(time/3600);
        resultStr += ":";
        if ((time%3600)/60 < 10)
            resultStr += "0";
        resultStr += QString::number((time%3600)/60);
        resultStr += ":";
        if (time%60 < 10)
            resultStr += "0";
        resultStr += QString::number(time%60);
    }

    return resultStr;
}

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
    allUse = 1;

    color = QColor(50+rand()%150, 50+rand()%150, 50+rand()%150);
}

void TimeUnit::Sort()
{
    int size = windowNames.size();
    for (int i = 0; i < size; i++)
    {
        for (int j = size-1; j > i; j--)
        {
            if (windowUse[j] > windowUse[j-1])
            {
                int temp = windowUse[j];
                windowUse[j] = windowUse[j-1];
                windowUse[j-1] = temp;
                QString tempstr = windowNames[j];
                windowNames[j] = windowNames[j-1];
                windowNames[j-1] = tempstr;
            }
        }
    }
}

void TimeUnit::AddUsage(QString windowName)
{
    bool existWin = false;
    int posWin = 0;
    int size = windowNames.size();
    for (int i = 0; i < size; i++)
    {
        if (windowNames.at(i) == windowName)
        {
            existWin = true;
            posWin = i;
        }
    }

    if (existWin)
    {
        // добавить с афк
        windowUse[posWin] = windowUse[posWin] + 1;
    }
    else
    {
        windowNames.append(windowName);
        windowUse.append(1);
    }

    allUse = allUse + 1;


    Sort();
}

QString TimeUnit::ToString(int pos)
{
    QString resultStr = "";
    resultStr += GetTime(windowUse[pos]);
    resultStr += " - ";
    resultStr += QString::number((int)((float)windowUse[pos]/(float)allUse*100.0)) + "%";
    resultStr += " - ";
    resultStr += windowNames[pos];

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
        resultStr += QString::number((time%3600)/60);
        resultStr += ":";
        if (time%60 < 10)
            resultStr += "0";
        resultStr += QString::number(time%60);
    }

    return resultStr;
}

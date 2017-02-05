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
    other = false;

    color = QColor(50+rand()%150, 50+rand()%150, 50+rand()%150);
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
}

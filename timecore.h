#ifndef TIMECORE_H
#define TIMECORE_H

#include "timeunit.h"
#include <windows.h>
#include <psapi.h>

class TimeCore
{
public:
    QList<TimeUnit> units;          // все процессы
    uchar *consumes;                // посекундный график
    int hours;                      // часы работы (для графика)

    bool afk;

public:
    TimeCore();

    void update();

    int GetProcess(QString *fullName, QString *windowName);
};

#endif // TIMECORE_H

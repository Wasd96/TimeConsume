#ifndef TIMECORE_H
#define TIMECORE_H

#include "timeunit.h"
#include <windows.h>
#include <psapi.h>

class TimeCore
{
public:
    QList<TimeUnit> units;          // все процессы
    int selected;                   // выбранный процесс
    uchar *consumes;                // посекундный график
    int hours;                      // часы работы (для графика)
    int secActive;                  // зарегистрированные секунды работы
    int secTemp;                    // ожидающие
    uchar *consumesTemp;            // ожидающий график

    bool afk;                       // юзер афк

public:
    TimeCore();

    void update();

    int getProcess(QString *fullName, QString *windowName);
};

#endif // TIMECORE_H

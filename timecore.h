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
    int secTemp;                    // для отката

    bool afk;                       // юзер афк

    QList<QString> statNames;       // для показа статистики
    QList<ulong> statUses;          //
    ulong statAllUse;               //

public:
    TimeCore();

    void update();
    void rollBack();                // откат из-за афк
    void ensure();                  // закрепление результатов (не откатятся)

    int getProcess(QString *fullName, QString *windowName);
};

#endif // TIMECORE_H

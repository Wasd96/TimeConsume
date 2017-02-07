#ifndef TIMEUNIT_H
#define TIMEUNIT_H

#include <QString>
#include <QList>
#include <QColor>

class TimeUnit
{
public:
    QString fullName;               // полный путь процесса
    QString processName;            // имя ехе-шника

    QList<QString> windowNames;     // имена окон для этого процесса
    QList<ulong> windowUse;         // использование каждого окна
    ulong allUse;                   // общее использование процесса

    QList<QString> windowNamesTemp; // буферные параметры
    QList<ulong> windowUseTemp;     // нужны для отката в случае afk
    ulong allUseTemp;               // а нужны ли они?..

    QColor color;

public:
    TimeUnit();
    TimeUnit(QString fullName, QString windowName);

    void Sort();
    void AddUsage(QString windowName);
    QString ToString(int pos);
    QString GetTime(int time);
};

#endif // TIMEUNIT_H

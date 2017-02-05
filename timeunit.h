#ifndef TIMEUNIT_H
#define TIMEUNIT_H

#include <QString>
#include <QList>

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

    bool other;                     // считать этот процесс незначительным

public:
    TimeUnit();
    TimeUnit(QString fullName, QString windowName);

    void AddUsage(QString windowName);
};

#endif // TIMEUNIT_H

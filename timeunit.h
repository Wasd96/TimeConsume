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

    QList<ulong> windowUseTemp;     // нужны для отката в случае afk
    ulong allUseTemp;               //

    QColor color;                   // цвет процесса

public:
    TimeUnit();
    TimeUnit(QString fullName, QString windowName);

    void RollBack();                // откат из-за афк
    void Ensure();                  // закрепление результатов (не откатятся)
    void Sort();                    // отстортировать окна процесса
    void AddUsage(QString windowName); // добавить окно
    QString ToString(int pos);         // окно в виде строки
    static QString GetTime(int time);  // время в виде строки
};

#endif // TIMEUNIT_H

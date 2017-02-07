#include "timecore.h"

#include <QDebug>

TimeCore::TimeCore()
{
    secActive = 0;
    selected = -1;
    consumes = new uchar[3600];
}

void TimeCore::update()
{
    QString fullName;
    QString winName;
    getProcess(&fullName, &winName);

    bool exist = false;
    for (int i = 0; i < units.size(); i++)
    {
        if (units.at(i).fullName == fullName)
        {
            exist = true;
            units[i].AddUsage(winName);
            consumes[secActive] = i;
        }
    }

    if (!exist)
    {
        TimeUnit TU(fullName, winName);
        units.append(TU);
        consumes[secActive] = units.size()-1;
    }

    secActive++;


    //qDebug() << fullName;
    //qDebug() << winName;
}



int TimeCore::getProcess(QString *fullName, QString *windowName)
{
    HWND hwnd = GetForegroundWindow();

    unsigned long magic = 5;
    LPDWORD process = (LPDWORD)&magic;
    if (hwnd != NULL)
    {
        GetWindowThreadProcessId(hwnd, process);
        if (process != NULL)
        {
            HANDLE hndl = OpenProcess(PROCESS_QUERY_INFORMATION, false, *process);
            WCHAR buff[200];
            if (hndl != NULL)
            {
                int len = GetProcessImageFileNameW(hndl, (LPWSTR)buff, 120);
                *fullName = QString::fromWCharArray(buff, len);
                CloseHandle(hndl);
            }
            else return -1;
        }
        else return -1;
    }
    else return -1;

    WCHAR buff[120];
    int len = GetWindowTextW(hwnd, (LPWSTR)buff, 120);
    *windowName = QString::fromWCharArray(buff, len);
    return 0;
}

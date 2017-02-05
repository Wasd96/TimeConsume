#include "timecore.h"

#include <QDebug>

TimeCore::TimeCore()
{

}

void TimeCore::update()
{
    QString fullName;
    QString winName;
    GetProcess(&fullName, &winName);

    bool exist = false;
    int pos = 0;
    for (int i = 0; i < units.size(); i++)
    {
        if (units.at(i).fullName == fullName)
        {
            exist = true;
            pos = i;
        }
    }
    if (exist)
    {
        units[pos].AddUsage(winName);

    }
    else
    {
        TimeUnit TU(fullName, winName);

        units.append(TU);
    }

    //qDebug() << fullName;
    //qDebug() << winName;
}

int TimeCore::GetProcess(QString *fullName, QString *windowName)
{
    HWND hwnd = GetForegroundWindow();

    unsigned long magic = 5;
    LPDWORD process = (LPDWORD)&magic;
    if (hwnd != NULL)
    {
        DWORD pro = GetWindowThreadProcessId(hwnd, process);
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

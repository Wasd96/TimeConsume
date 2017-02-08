#include "timecore.h"

#include <QDebug>

TimeCore::TimeCore()
{
    secActive = 0;
    hours = 1;
    selected = -1;
    consumes = new uchar[3600];
}

void TimeCore::update()
{
    QString fullName;
    QString winName;
    getProcess(&fullName, &winName);    // получение активного процесса

    bool exist = false;
    for (int i = 0; i < units.size(); i++)
    {
        if (units.at(i).fullName == fullName)
        {
            exist = true;               // если такой был
            units[i].AddUsage(winName); // обновляем
            consumes[secActive] = i;
            break;
        }
    }

    if (!exist)                         // не было
    {
        TimeUnit TU(fullName, winName);
        units.append(TU);               // добавляем
        consumes[secActive] = units.size()-1;
    }

    secActive++;
    secTemp++;

    if ((secActive+1)/3600 >= hours)    // шкала заполнилась
    {
        uchar *temp = new uchar[hours*3600]; // выделяем временное хранилище
        for (int i = 0; i < secActive; i++)
        {
            temp[i] = consumes[i]; // копируем
        }
        hours++;
        delete[] consumes;
        consumes = new uchar[hours*3600]; // выделяем еще один час
        for (int i = 0; i < secActive; i++)
        {
            consumes[i] = temp[i];
        }
        delete[] temp;
    }

}

void TimeCore::rollBack() // откат
{
    secActive -= secTemp;
    secTemp = 0;
    for (int i = 0; i < units.size(); i++)
        units[i].RollBack();
}

void TimeCore::ensure() // закрепление
{
    for (int i = 0; i < units.size(); i++)
        units[i].Ensure();
    secTemp = 0;
}



int TimeCore::getProcess(QString *fullName, QString *windowName)
{
    HWND hwnd = GetForegroundWindow();  // получить активное окно

    unsigned long magic = 5;            // магия вин апи
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
                int len = GetProcessImageFileNameW(hndl, (LPWSTR)buff, 200);
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

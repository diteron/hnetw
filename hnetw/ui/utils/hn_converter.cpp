#include <stdafx.h>
#include "hn_converter.h"

QString HnConverter::uint32ToIpString(uint32_t ip)
{
    char strIpBuffer[16];
    in_addr addr;
    addr.s_addr = ip;
    inet_ntop(AF_INET, &addr, strIpBuffer, 16);
    return QString(strIpBuffer);
}

QString HnConverter::msSinceMidntUtcToTimeString(uint32_t milliseconds)
{
    QString timeString = "";

    int ms = milliseconds % 1000;
    timeString.append(QString::number(ms) + " seconds after midnight UTC");
    milliseconds /= 1000;

    int sec = milliseconds % 60;
    timeString.prepend(QString::number(sec) + ".");
    milliseconds /= 60;

    int min = milliseconds % 60;
    timeString.prepend(QString::number(min) + " minutes, ");
    milliseconds /= 60;

    int hours = milliseconds % 24;
    timeString.prepend(QString::number(hours) + " hours, ");
    milliseconds /= 24;

    int days = milliseconds;
    timeString.prepend(QString::number(days) + " days, ");

    return timeString;
}

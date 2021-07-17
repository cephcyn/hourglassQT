#include "timer.h"

#include <QtDebug>

Timer::Timer()
{
    // Default constructor creates an empty invalid timer
    Timer::input = "";
    Timer::valid = false;
    Timer::totalHours = 0;
    Timer::totalMinutes = 0;
    Timer::totalSeconds = 0;
    Timer::remainHours = 0;
    Timer::remainMinutes = 0;
    Timer::remainSeconds = 0;
}

Timer::Timer(QString input)
{
    Timer::input = input;
    // TODO do a proper check for validity / parsing the input
//    Timer::valid = false;
    // TODO this is a temporary default interpretation
    Timer::valid = true;
    Timer::totalHours = 0;
    Timer::totalMinutes = 0;
    Timer::totalSeconds = 30;
    Timer::remainHours = 0;
    Timer::remainMinutes = 0;
    Timer::remainSeconds = 30;
    qDebug() << "constructing Timer...";
    qDebug() << "            raw input: " << input;
    qDebug() << "parsed total duration: " << Timer::text_total_duration();
}

bool Timer::is_valid()
{
    return Timer::valid;
}

QString Timer::text_total_duration()
{
    return QString::number(Timer::totalHours) + ":"
            + QString::number(Timer::totalMinutes) + ":"
            + QString::number(Timer::totalSeconds);
}

QString Timer::text_remain_duration()
{
    return QString::number(Timer::remainHours) + ":"
            + QString::number(Timer::remainMinutes) + ":"
            + QString::number(Timer::remainSeconds);
}

quint64 Timer::int_total_duration()
{
    return (60 * 60 * Timer::totalHours)
            + (60 * Timer::totalMinutes)
            + (1 * Timer::totalSeconds);
}

quint64 Timer::int_remain_duration()
{
    return (60 * 60 * Timer::remainHours)
            + (60 * Timer::remainMinutes)
            + (1 * Timer::remainSeconds);
}

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
    return
            QString::number(Timer::totalHours) + ":" +
            QString::number(Timer::totalMinutes) + ":" +
            QString::number(Timer::totalSeconds);
}

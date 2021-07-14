#include "timer.h"

#include <QtDebug>

Timer::Timer(QString input)
{
    qDebug() << "constructing Timer...";
    qDebug() << "  raw input: " << input;
    Timer::input = input;
    // TODO check for validity properly
    Timer::valid = false;
    // TODO
    qDebug() << "parsed time: " << input;
}

bool Timer::is_valid()
{
    return Timer::valid;
}

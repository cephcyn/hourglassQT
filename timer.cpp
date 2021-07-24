#include "timer.h"

#include <QtDebug>
#include <QRegularExpression>
#include <QTimer>

Timer::Timer()
{
    // Default constructor creates an empty invalid timer
    Timer::input = "";
    Timer::type = TimerType::duration;
    Timer::valid = false;
    Timer::totalHours = 0;
    Timer::totalMinutes = 0;
    Timer::totalSeconds = 0;
    Timer::construct_finish();
}

Timer::Timer(QString input)
{
    Timer::input = input.simplified();;
    // TODO do a proper check for validity / parsing the input
    // TODO this is a temporary default interpretation
    Timer::valid = true;
    Timer::type = TimerType::duration;
    Timer::totalHours = 0;
    Timer::totalMinutes = 0;
    Timer::totalSeconds = 5;
    // number-only minutes input
    // example: "3" -> 0:03:00 duration
    QRegularExpression reMinuteOnly = QRegularExpression("^[0-9]+$");
    qDebug() << Timer::input << "; minuteOnly =" << reMinuteOnly.match(Timer::input).hasMatch();
    if (reMinuteOnly.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        Timer::totalHours = 0;
        Timer::totalMinutes = Timer::input.toULongLong();
        Timer::totalSeconds = 0;
    }
    // TODO remove...?
    Timer::construct_finish();
}

void Timer::construct_finish()
{
    // do the constructor stuff that always needs to be done
    Timer::isStarted = false;
    Timer::isRunning = false;
    Timer::isFinished = false;
    Timer::remainHours = Timer::totalHours;
    Timer::remainMinutes = Timer::totalMinutes;
    Timer::remainSeconds = Timer::totalSeconds;
    qDebug() << "constructing Timer...";
    qDebug() << "            raw input: " << Timer::input;
    static std::array<const char*, 2> inputEnumNames = {"duration", "alarm"};
    qDebug() << "          parsed type: " << inputEnumNames[Timer::type];
    qDebug() << "parsed total duration: " << Timer::text_total_duration();
    qDebug() << "   parsed finish time: " << "TODO not-done-yet";
}

void Timer::trigger_start()
{
//    qDebug() << "called timer::trigger_start";
    if (! Timer::is_valid())
    {
        return;
    }
    Timer::remainHours = Timer::totalHours;
    Timer::remainMinutes = Timer::totalMinutes;
    Timer::remainSeconds = Timer::totalSeconds;
    Timer::isStarted = true;
    Timer::isRunning = true;
    Timer::isFinished = false;
}

void Timer::trigger_toggle()
{
//    qDebug() << "called timer::trigger_toggle";
    if (! Timer::is_valid())
    {
        return;
    }
    if ((! Timer::is_started()) or (Timer::is_finished())) {
        // we are really "start"ing the timer
        Timer::trigger_start();
    }
    else
    {
        // we are pausing/unpausing the timer
        Timer::isStarted = true;
        Timer::isRunning = ! Timer::isRunning;
        Timer::isFinished = false;
    }
}

void Timer::trigger_reset()
{
//    qDebug() << "called timer::trigger_reset";
    if (! Timer::is_valid())
    {
        return;
    }
    Timer::isStarted = false;
    Timer::isRunning = false;
    Timer::isFinished = false;
    Timer::remainHours = Timer::totalHours;
    Timer::remainMinutes = Timer::totalMinutes;
    Timer::remainSeconds = Timer::totalSeconds;
}

void Timer::increment_second()
{
    if (! Timer::is_valid())
    {
        return;
    }
    // if the timer just reached 0, update timer flags...
    if (Timer::int_remain_duration() == 0)
    {
        if (Timer::is_running())
        {
            // finished "alarm" is going off
            Timer::isFinished = true;
        }
        Timer::isRunning = false;
    }
    // decrement current remaining time if we're currently running
    if (Timer::is_running())
    {
        if (Timer::remainSeconds > 0) {
            Timer::remainSeconds -= 1;
        } else if (Timer::remainMinutes > 0) {
            Timer::remainMinutes -= 1;
            Timer::remainSeconds = 59;
        } else {
            Timer::remainHours -= 1;
            Timer::remainMinutes = 59;
            Timer::remainSeconds = 59;
        }
    }
}

bool Timer::is_valid()
{
    return Timer::valid;
}

bool Timer::is_started()
{
    return Timer::isStarted;
}

bool Timer::is_running()
{
    return Timer::isRunning;
}

bool Timer::is_finished()
{
    return Timer::isFinished;
}

QString Timer::text_total_duration()
{
    return QString::number(Timer::totalHours) + ":"
            + ("00" + QString::number(Timer::totalMinutes)).right(2) + ":"
            + ("00" + QString::number(Timer::totalSeconds)).right(2);
}

QString Timer::text_remain_duration()
{
    return QString::number(Timer::remainHours) + ":"
            + ("00" + QString::number(Timer::remainMinutes)).right(2) + ":"
            + ("00" + QString::number(Timer::remainSeconds)).right(2);
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

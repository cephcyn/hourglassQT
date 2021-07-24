#include "timer.h"

#include <QtDebug>
#include <QRegularExpression>
#include <QtMath>
#include <QTimer>

Timer::Timer()
{
    // Default constructor creates an empty invalid timer
    Timer::input = "";
    Timer::valid = false;
    Timer::type = TimerType::duration;
    Timer::totalHours = 0;
    Timer::totalMinutes = 0;
    Timer::totalSeconds = 0;
    Timer::construct_finish();
}

Timer::Timer(QString input)
{
    Timer::input = input.simplified();

    // number-only (zero or positive, integer) of minutes
    // example: "3" -> 0:03:00 duration
    QRegularExpression reMinuteOnly = QRegularExpression("^[0-9]+$");
    qDebug() << Timer::input << "; mo =" << reMinuteOnly.match(Timer::input).hasMatch();
    // short form duration (using :s) mm:ss, hh:mm:ss
    // example: "2:40" -> 0:02:40 duration
    // example: "132:03:25" -> 132:03:25 duration
    QRegularExpression reShortDurationColon = QRegularExpression("^([0-9]+:)?[0-9]+:[0-9]+$");
    qDebug() << Timer::input << "; sdc =" << reShortDurationColon.match(Timer::input).hasMatch();
    // short form duration (using .s) mm.ss, hh.mm.ss
    // example: "2.40" -> 0:02:40 duration
    // example: "132.03.25" -> 132:03:25 duration
    QRegularExpression reShortDurationPeriod = QRegularExpression("^([0-9]+\\.)?[0-9]+\\.[0-9]+$");
    qDebug() << Timer::input << "; sdp =" << reShortDurationPeriod.match(Timer::input).hasMatch();

    // now go through and check if the input matches any supported format...
    if (reMinuteOnly.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        Timer::totalHours = 0;
        Timer::totalMinutes = Timer::input.toULongLong();
        Timer::totalSeconds = 0;
    }
    else if (reShortDurationColon.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        QStringList pieces = Timer::input.split(":");
        if (pieces.size() == 2)
        {
            // only has minutes and seconds
            Timer::totalHours = 0;
            Timer::totalMinutes = pieces[0].toULongLong();
            Timer::totalSeconds = pieces[1].toULongLong();
        }
        else
        {
            // has hours, minutes, and seconds
            Timer::totalHours = pieces[0].toULongLong();
            Timer::totalMinutes = pieces[1].toULongLong();
            Timer::totalSeconds = pieces[2].toULongLong();
        }
    }
    else if (reShortDurationPeriod.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        QStringList pieces = Timer::input.split(".");
        if (pieces.size() == 2)
        {
            // only has minutes and seconds
            Timer::totalHours = 0;
            Timer::totalMinutes = pieces[0].toULongLong();
            Timer::totalSeconds = pieces[1].toULongLong();
        }
        else
        {
            // has hours, minutes, and seconds
            Timer::totalHours = pieces[0].toULongLong();
            Timer::totalMinutes = pieces[1].toULongLong();
            Timer::totalSeconds = pieces[2].toULongLong();
        }
    }
//    else if (false)
//    {
//        // TODO
//        Timer::valid = true;
//        Timer::type = TimerType::duration;
//        double numMinutes = Timer::input.toDouble();
//        Timer::totalHours = 0;
//        Timer::totalMinutes = qFloor(numMinutes); // Timer::input.toULongLong();
//        Timer::totalSeconds = qFloor(60 * (numMinutes - Timer::totalMinutes));
//    }
    else
    {
        // invalid input
        Timer::valid = false;
        Timer::type = TimerType::duration;
        Timer::totalHours = 0;
        Timer::totalMinutes = 0;
        Timer::totalSeconds = 0;
    }
    Timer::construct_finish();
}

void Timer::construct_finish()
{
    // do the constructor stuff that always needs to be done
    // TODO normalize the remainHours, remainMinutes, and remainSeconds to have max 60 in min and sec
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

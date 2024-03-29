#include "timer.h"

#include <QtDebug>
#include <QRegularExpression>
#include <QtMath>
#include <QTime>

Timer::Timer()
{
    // Create an empty-input invalid timer
    Timer::input = "";
    Timer::construct_invalid();
    Timer::construct_finish();
}

Timer::Timer(QString input)
{
    // trim whitespace from ends of input
    // also reduce whitespace between tokens to single space
    Timer::input = input.simplified();

    // number-only (zero or positive, integer) of minutes
    // example: "3" -> 0:03:00 duration
    QRegularExpression reMinuteOnlyDuration = QRegularExpression(
                "^[0-9]+$"
    );
    qDebug() << Timer::input << "; mod =" << reMinuteOnlyDuration.match(Timer::input).hasMatch();
    // short form duration mm:ss, hh:mm:ss, mm.ss, hh.mm.ss
    // example: "2:40" -> 0:02:40 duration
    // example: "132:03:25" -> 132:03:25 duration
    // example: "2.40" -> 0:02:40 duration
    // example: "132.03.25" -> 132:03:25 duration
    QRegularExpression reShortDuration = QRegularExpression(
                "^[0-9]+([:.])[0-9]+(\\1[0-9]+)?$"
    );
    qDebug() << Timer::input << "; sd =" << reShortDuration.match(Timer::input).hasMatch();
    // specified units with seconds/seconds/secs/sec/s, minutes/minute/mins/min/m, hours/hour/hrs/hr/h
    // can take decimal input as well
    // also can take combined units using multiple of above timespans
    // example: "30 seconds" -> 0:00:30 duration
    // example: "5 m" -> 0:05:00 duration
    // example: "10.5h" -> 10:30:00 duration
    // example: "5 minutes 20 seconds" -> 0:05:20 duration
    // example: "7 h 15.5 m" -> 7:15:30 duration
    // example: "2h2m2s" -> 2:02:02 duration
    // TODO add support for days/d, weeks/w, months/mo, years/y ?
    QRegularExpression reUnitDuration = QRegularExpression(
                "^(([0-9]*\\.?[0-9]+) *(seconds|second|secs|sec|s|minutes|minute|mins|min|m|hours|hour|hrs|hr|h) *)+$"
    );
    qDebug() << Timer::input << "; ud =" << reUnitDuration.match(Timer::input).hasMatch();
    // clock time (using : or .) with am/pm, soonest time that matches today or tomorrow
    // example: "2 pm" -> soonest 14:00:00 alarm
    // example: "2:30 pm" -> soonest 14:30:00 alarm
    // example: "2:30:15 pm" -> soonest 14:30:15 alarm
    // example: "3pm" -> soonest 15:00:00 alarm
    // example: "2 pm" -> soonest 14:00:00 alarm
    // example: "2.30 pm" -> soonest 14:30:00 alarm
    // example: "2.30.15 pm" -> soonest 14:30:15 alarm
    // example: "3pm" -> soonest 15:00:00 alarm
    QRegularExpression reClockTime = QRegularExpression(
                "^[0-9]+(([:.])[0-9][0-9])?(\\2[0-9][0-9])? *(am|pm)$"
    );
    qDebug() << Timer::input << "; ctc =" << reClockTime.match(Timer::input).hasMatch();
    // TODO be able to input 24h format time of day somehow?
    // TODO be able to input date somehow?
    // TODO be able to input date via weekday label somehow?
    // TODO be able to input a combination of date and time somehow?

    // now go through and check if the input matches any supported format
    if (reMinuteOnlyDuration.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        Timer::totalHour = 0;
        Timer::totalMinute = Timer::input.toULongLong();
        Timer::totalSecond = 0;
    }
    else if (reShortDuration.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        QStringList pieces = Timer::input.split(QRegularExpression("[:.]"));
        if (pieces.size() == 2)
        {
            // only has minutes and seconds
            Timer::totalHour = 0;
            Timer::totalMinute = pieces[0].toULongLong();
            Timer::totalSecond = pieces[1].toULongLong();
        }
        else
        {
            // has hours, minutes, and seconds
            Timer::totalHour = pieces[0].toULongLong();
            Timer::totalMinute = pieces[1].toULongLong();
            Timer::totalSecond = pieces[2].toULongLong();
        }
    }
    else if (reUnitDuration.match(Timer::input).hasMatch())
    {
        Timer::valid = true;
        Timer::type = TimerType::duration;
        // initialize timer data to 0, we'll increment it later
        Timer::totalHour = 0;
        Timer::totalMinute = 0;
        Timer::totalSecond = 0;
        // start parsing the input chunks
        QString inputRemaining = Timer::input;
        QRegularExpressionMatch inputRemainingMatch = reUnitDuration.match(inputRemaining);
        while (inputRemainingMatch.hasMatch())
        {
            // captured(1) is the entire rightmost time/unit chunk
            // captured(2) is the rightmost number
            // captured(3) is the rightmost unit
            // parse rightmost chunk into the number and unit sections
            QString subNum = inputRemainingMatch.captured(2);
            QString subUnit = inputRemainingMatch.captured(3);
            double doubleNum = subNum.toDouble();
            // take the number of units and turn them into duration info
            if (QRegularExpression("^(seconds|second|secs|sec|s)$").match(subUnit).hasMatch())
            {
                // unit chunk describes seconds
                quint64 chunkSecond = qFloor(doubleNum);
                // increment timer total data
                Timer::totalSecond = Timer::totalSecond + chunkSecond;
            }
            else if (QRegularExpression("^(minutes|minute|mins|min|m)$").match(subUnit).hasMatch())
            {
                // unit chunk describes minutes
                quint64 chunkMinute = qFloor(doubleNum);
                doubleNum = 60 * (doubleNum - chunkMinute); // convert to seconds
                quint64 chunkSecond = qFloor(doubleNum);
                // increment timer total data
                Timer::totalMinute = Timer::totalMinute + chunkMinute;
                Timer::totalSecond = Timer::totalSecond + chunkSecond;
            }
            else if (QRegularExpression("^(hours|hour|hrs|hr|h)$").match(subUnit).hasMatch())
            {
                // unit chunk describes hours
                quint64 chunkHour = qFloor(doubleNum);
                doubleNum = 60 * (doubleNum - chunkHour); // convert to minutes
                quint64 chunkMinute = qFloor(doubleNum);
                doubleNum = 60 * (doubleNum - chunkMinute); // convert to seconds
                quint64 chunkSecond = qFloor(doubleNum);
                // increment timer total data
                Timer::totalHour = Timer::totalHour + chunkHour;
                Timer::totalMinute = Timer::totalMinute + chunkMinute;
                Timer::totalSecond = Timer::totalSecond + chunkSecond;
            }
            // remove the rightmost chunk and repeat...
            inputRemaining = inputRemaining.left(inputRemaining.length()-inputRemainingMatch.captured(1).length());
            inputRemainingMatch = reUnitDuration.match(inputRemaining);
        }
    }
    else if (reClockTime.match(Timer::input).hasMatch())
    {
        // cut into time and am/pm sections
        QStringList subTimeStrs = Timer::input.left(Timer::input.length()-2).simplified().split(QRegularExpression("[:.]"));
        QString subAmPm = Timer::input.right(2);
        // parse subTime into actual numbers
        quint64 subTimes [3] = { 0, 0, 0 };
        subTimes[0] = subTimeStrs[0].toULongLong();
        if (subTimeStrs.size() > 1)
        {
            subTimes[1] = subTimeStrs[1].toULongLong();
        }
        if (subTimeStrs.size() > 2)
        {
            subTimes[2] = subTimeStrs[2].toULongLong();
        }
        // check if the user-passed timestamp actually makes sense
        if ((subTimes[0] >= 1) and (subTimes[0] <= 12) and
                (subTimes[1] <= 59) and
                (subTimes[2] <= 59))
        {
            // the timestamp is valid!
            // normalize the time to 24-hour (hour in 0-23 range) style
            quint64 newHour = subTimes[0];
            if (subAmPm.toLower() == "pm")
            {
                newHour = subTimes[0] + 12;
            }
            if (subTimes[0] == 12)
            {
                newHour = newHour - 12;
            }
            subTimes[0] = newHour;
            // and set the actual times now
            Timer::valid = true;
            Timer::type = TimerType::alarm;
            Timer::clockHour = subTimes[0];
            Timer::clockMinute = subTimes[1];
            Timer::clockSecond = subTimes[2];
        }
        else
        {
            // timestamp is invalid
            Timer::construct_invalid();
        }
    }
    else
    {
        // invalid input
        Timer::construct_invalid();
    }
    Timer::construct_finish();
}

void Timer::normalize()
{
    // make sure the base clock* and total* fields are matching up
    QTime currentTime = QTime::currentTime();
    // do handling of duration vs alarm timertype
    if (Timer::type == TimerType::duration)
    {
        // set the clock fields to match total duration fields
        QTime targetTime = currentTime.addSecs(
                    (60 * 60 * Timer::totalHour) + (60 * Timer::totalMinute) + (Timer::totalSecond)
        );
        Timer::clockHour = targetTime.hour();
        Timer::clockMinute = targetTime.minute();
        Timer::clockSecond = targetTime.second();
    }
    else
    {
        // set the total duration fields to match clock fields
        QTime targetTime = QTime(Timer::clockHour, Timer::clockMinute, Timer::clockSecond);
        int secsDifference = currentTime.secsTo(targetTime);
        if (secsDifference < 0)
        {
            // assume next day!
            secsDifference = secsDifference + (24 * 60 * 60);
        }
        Timer::totalHour = 0;
        Timer::totalMinute = 0;
        Timer::totalSecond = secsDifference;
    }

    // make sure the totalHours, totalMinutes, and totalSeconds are normalized
    if (Timer::totalSecond >= 60)
    {
        Timer::totalMinute = Timer::totalMinute + qFloor(Timer::totalSecond / 60);
        Timer::totalSecond = Timer::totalSecond % 60;
    }
    if (Timer::totalMinute >= 60)
    {
        Timer::totalHour = Timer::totalHour + qFloor(Timer::totalMinute / 60);
        Timer::totalMinute = Timer::totalMinute % 60;
    }
}

void Timer::construct_invalid()
{
    // fill in details that describe this timer as invalid
    Timer::valid = false;
    Timer::type = TimerType::duration;
    Timer::totalHour = 0;
    Timer::totalMinute = 0;
    Timer::totalSecond = 0;
}

void Timer::construct_finish()
{
    // normalize the timer data
    Timer::normalize();
    // now set the common fields
    Timer::isStarted = false;
    Timer::isRunning = false;
    Timer::isFinished = false;
    Timer::remainHour = Timer::totalHour;
    Timer::remainMinute = Timer::totalMinute;
    Timer::remainSecond = Timer::totalSecond;
    qDebug() << "constructing Timer...";
    qDebug() << "            raw input: " << Timer::input;
    static std::array<const char*, 2> inputEnumNames = {"duration", "alarm"};
    qDebug() << "          parsed type: " << inputEnumNames[Timer::type];
    qDebug() << "parsed total duration: " << Timer::text_total_duration();
    qDebug() << "   parsed finish time: " << Timer::text_clock_24();
}

void Timer::trigger_start()
{
//    qDebug() << "called timer::trigger_start";
    if (! Timer::is_valid())
    {
        return;
    }
    // normalize the timer origin data
    Timer::normalize();
    // update timer status data
    Timer::remainHour = Timer::totalHour;
    Timer::remainMinute = Timer::totalMinute;
    Timer::remainSecond = Timer::totalSecond;
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
    // normalize the timer origin data
    Timer::normalize();
    // update timer status data
    Timer::isStarted = false;
    Timer::isRunning = false;
    Timer::isFinished = false;
    Timer::remainHour = Timer::totalHour;
    Timer::remainMinute = Timer::totalMinute;
    Timer::remainSecond = Timer::totalSecond;
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
        if (Timer::remainSecond > 0) {
            Timer::remainSecond -= 1;
        } else if (Timer::remainMinute > 0) {
            Timer::remainMinute -= 1;
            Timer::remainSecond = 59;
        } else {
            Timer::remainHour -= 1;
            Timer::remainMinute = 59;
            Timer::remainSecond = 59;
        }
    }
}

bool Timer::matches_input(QString text)
{
    return text.simplified() == Timer::input;
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
    return QString::number(Timer::totalHour) + ":"
            + ("00" + QString::number(Timer::totalMinute)).right(2) + ":"
            + ("00" + QString::number(Timer::totalSecond)).right(2);
}

QString Timer::text_clock_24()
{
    return ("00" + QString::number(Timer::clockHour)).right(2) + ":"
            + ("00" + QString::number(Timer::clockMinute)).right(2) + ":"
            + ("00" + QString::number(Timer::clockSecond)).right(2);
}

QString Timer::text_remain_duration()
{
    return QString::number(Timer::remainHour) + ":"
            + ("00" + QString::number(Timer::remainMinute)).right(2) + ":"
            + ("00" + QString::number(Timer::remainSecond)).right(2);
}

quint64 Timer::int_total_duration()
{
    return (60 * 60 * Timer::totalHour)
            + (60 * Timer::totalMinute)
            + (1 * Timer::totalSecond);
}

quint64 Timer::int_remain_duration()
{
    return (60 * 60 * Timer::remainHour)
            + (60 * Timer::remainMinute)
            + (1 * Timer::remainSecond);
}

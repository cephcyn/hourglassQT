#ifndef TIMER_H
#define TIMER_H

#include <QtGlobal>
#include <QString>

enum TimerType { duration, alarm };

class Timer
{
public:
    Timer();
    Timer(QString input);

    void trigger_start();
    void trigger_toggle();
    void trigger_reset();

    void increment_second();

    bool matches_input(QString text);

    bool is_valid();
    bool is_started();
    bool is_running();
    bool is_finished();

    QString text_total_duration();
    QString text_remain_duration();
    QString text_clock_24();

    quint64 int_total_duration();
    quint64 int_remain_duration();

private:
    QString input;
    bool valid;
    TimerType type;
    bool isStarted;
    bool isRunning;
    bool isFinished;

    quint64 totalHour;
    quint64 totalMinute;
    quint64 totalSecond;

    quint64 clockHour;
    quint64 clockMinute;
    quint64 clockSecond;

    quint64 remainHour;
    quint64 remainMinute;
    quint64 remainSecond;

    void normalize();
    void construct_invalid();
    void construct_finish();
};

#endif // TIMER_H

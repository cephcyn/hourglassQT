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

    quint64 int_total_duration();
    quint64 int_remain_duration();

private:
    QString input;
    bool valid;
    TimerType type;
    bool isStarted;
    bool isRunning;
    bool isFinished;

    quint64 totalHours;
    quint64 totalMinutes;
    quint64 totalSeconds;

    quint64 remainHours;
    quint64 remainMinutes;
    quint64 remainSeconds;

    void construct_finish();
};

#endif // TIMER_H

#ifndef TIMER_H
#define TIMER_H

#include <QtGlobal>
#include <QString>

class Timer
{
public:
    Timer();
    Timer(QString input);

    bool is_valid();

    QString text_total_duration();
    QString text_remain_duration();

    quint64 int_total_duration();
    quint64 int_remain_duration();

private:
    QString input;
    bool valid;

    quint64 totalHours;
    quint64 totalMinutes;
    quint64 totalSeconds;

    quint64 remainHours;
    quint64 remainMinutes;
    quint64 remainSeconds;

};

#endif // TIMER_H

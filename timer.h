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

private:
    QString input;
    bool valid;

    quint64 totalHours;
    quint64 totalMinutes;
    quint64 totalSeconds;

};

#endif // TIMER_H

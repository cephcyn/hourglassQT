#ifndef TIMER_H
#define TIMER_H

#include <QtGlobal>
#include <QString>

class Timer
{
public:
    Timer(QString input);

    bool is_valid();

private:
    bool valid;
    QString input;

    quint64 timerSeconds;
    quint64 timerMinutes;
    quint64 timerHours;

};

#endif // TIMER_H

#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QMainWindow>

#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TimerWindow; }
QT_END_NAMESPACE

class TimerWindow : public QMainWindow
{
    Q_OBJECT

public:
    TimerWindow(QWidget *parent = nullptr);
    ~TimerWindow();

private slots:
    void on_inputTime_returnPressed();

    void on_pushToggle_clicked();

    void on_pushReset_clicked();

private:
    Ui::TimerWindow *ui;

    Timer timer;

};
#endif // TIMERWINDOW_H

#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TimerWindow; }
QT_END_NAMESPACE

class TimerWindow : public QMainWindow
{
    Q_OBJECT

public:
    TimerWindow(QWidget *parent = nullptr);
    ~TimerWindow();

private:
    Ui::TimerWindow *ui;
};
#endif // TIMERWINDOW_H

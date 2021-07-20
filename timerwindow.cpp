#include <QtDebug>
#include <QTimer>

#include "timerwindow.h"
#include "ui_timerwindow.h"
#include "timer.h"

TimerWindow::TimerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimerWindow)
{
    TimerWindow::ui->setupUi(this);
    TimerWindow::timer = new Timer();
    TimerWindow::qtimer = new QTimer(this);
    connect(TimerWindow::qtimer, &QTimer::timeout, this, &TimerWindow::update_timer);
    // TODO figure out how to get the timer to work with toggle, reset
}

TimerWindow::~TimerWindow()
{
    delete ui;
    delete timer;
    delete qtimer;
}

void TimerWindow::on_inputTime_returnPressed()
{
    // TODO
    qDebug() << "inputTime pressed return...";
    // clean up previous window state
    TimerWindow::ui->pushToggle->setText("Start");
    TimerWindow::ui->progressBar->setMaximum(1);
    TimerWindow::ui->progressBar->setValue(0);
    // parse the user input
    TimerWindow::timer = new Timer(TimerWindow::ui->inputTime->text());
    if (! TimerWindow::timer->is_valid())
    {
        // TODO
        qDebug() << "input parsed invalid";
    }
    else
    {
        // TODO
        qDebug() << "input parsed valid";
        TimerWindow::timer->trigger_start();
        TimerWindow::rerender();
        TimerWindow::qtimer->start(1000);
    }
}

void TimerWindow::update_timer()
{
    // TODO
    qDebug() << "update";
    TimerWindow::timer->increment_second();
    TimerWindow::ui->progressBar->setMaximum(
                TimerWindow::timer->int_total_duration()
    );
    TimerWindow::ui->progressBar->setValue(
                TimerWindow::timer->int_remain_duration()
    );
    if (TimerWindow::timer->is_finished())
    {
        qDebug() << "timer alarm went off";
        TimerWindow::qtimer->stop();
    }
}

void TimerWindow::rerender()
{
    // TODO clean text display up?
    if (TimerWindow::timer->is_started())
    {
        if (TimerWindow::timer->is_running())
        {
            TimerWindow::ui->pushToggle->setText("Pause");
        }
        else
        {
            TimerWindow::ui->pushToggle->setText("Resume");
        }
        TimerWindow::ui->progressBar->setMaximum(
                    TimerWindow::timer->int_total_duration()
        );
        TimerWindow::ui->progressBar->setValue(
                    TimerWindow::timer->int_remain_duration()
        );
    }
    else
    {
        TimerWindow::ui->pushToggle->setText("Start");
        TimerWindow::ui->progressBar->setMaximum(1);
        TimerWindow::ui->progressBar->setValue(0);
    }
}

void TimerWindow::on_pushToggle_clicked()
{
    // TODO
    qDebug() << "pushToggle pressed... timer:is_running=" << TimerWindow::timer->is_running();
    TimerWindow::timer->trigger_toggle();
    TimerWindow::rerender();
}

void TimerWindow::on_pushReset_clicked()
{
    // TODO
    qDebug() << "pushReset pressed...";
    TimerWindow::timer->trigger_reset();
    TimerWindow::rerender();
}

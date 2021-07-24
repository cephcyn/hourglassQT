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
    // TODO there's gotta be a cleaner way to do this...
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
        TimerWindow::qtimer->start(1000);
        TimerWindow::rerender();
    }
}

void TimerWindow::update_timer()
{
    // TODO
    TimerWindow::timer->increment_second();
    if (TimerWindow::timer->is_finished())
    {
        qDebug() << "timer alarm went off";
        TimerWindow::qtimer->stop();
    }
    TimerWindow::rerender();
}

void TimerWindow::rerender()
{
    // TODO clean text display up?
    if (! TimerWindow::timer->is_valid())
    {
        // update progressbar to invalid state
        QPalette p = TimerWindow::ui->progressBar->palette();
        p.setColor(QPalette::Highlight, Qt::red);
        TimerWindow::ui->progressBar->setPalette(p);
        TimerWindow::ui->progressBar->setMaximum(1);
        TimerWindow::ui->progressBar->setValue(1);
        TimerWindow::ui->progressBar->setFormat(
                    "Input invalid"
        );
    }
    else if (TimerWindow::timer->is_started())
    {
        if (TimerWindow::timer->is_finished())
        {
            // update progressbar to full / alarm state
            QPalette p = TimerWindow::ui->progressBar->palette();
            p.setColor(QPalette::Highlight, Qt::darkRed);
            TimerWindow::ui->progressBar->setPalette(p);
            TimerWindow::ui->progressBar->setMaximum(1);
            TimerWindow::ui->progressBar->setValue(1);
            TimerWindow::ui->progressBar->setFormat(
                        "Timer done"
            );
            // TODO alarm is going off
            TimerWindow::ui->pushToggle->setText("Start");
        }
        else
        {
            // update progressbar to timer run state
            QPalette p = TimerWindow::ui->progressBar->palette();
            p.setColor(QPalette::Highlight, Qt::darkGreen);
            TimerWindow::ui->progressBar->setPalette(p);
            TimerWindow::ui->progressBar->setMaximum(
                        TimerWindow::timer->int_total_duration()
            );
            TimerWindow::ui->progressBar->setValue(
                        TimerWindow::timer->int_remain_duration()
            );
            TimerWindow::ui->progressBar->setFormat(
                        TimerWindow::timer->text_remain_duration()
            );
            if (TimerWindow::timer->is_running())
            {
                TimerWindow::ui->pushToggle->setText("Pause");
            }
            else
            {
                TimerWindow::ui->pushToggle->setText("Resume");
            }
        }
    }
    else
    {
        TimerWindow::ui->pushToggle->setText("Start");
        // update progressbar to non-running state
        TimerWindow::ui->progressBar->setMaximum(1);
        TimerWindow::ui->progressBar->setValue(0);
        TimerWindow::ui->progressBar->setFormat("");
    }
}

void TimerWindow::on_pushToggle_clicked()
{
    // TODO
    qDebug() << "pushToggle pressed... timer:is_running=" << TimerWindow::timer->is_running();
    TimerWindow::timer->trigger_toggle();
    if (TimerWindow::timer->is_running())
    {
        TimerWindow::qtimer->start(1000);
    }
    else
    {
        TimerWindow::qtimer->stop();
    }
    TimerWindow::rerender();
}

void TimerWindow::on_pushReset_clicked()
{
    // TODO
    qDebug() << "pushReset pressed...";
    TimerWindow::timer->trigger_reset();
    TimerWindow::qtimer->stop();
    TimerWindow::rerender();
}

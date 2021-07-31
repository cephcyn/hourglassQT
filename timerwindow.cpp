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
}

TimerWindow::~TimerWindow()
{
    delete ui;
    delete timer;
    delete qtimer;
}

void TimerWindow::on_inputTime_returnPressed()
{
    // clean up previous window state
    // TODO there's gotta be a cleaner way to do this?
    TimerWindow::ui->pushToggle->setText("Start");
    TimerWindow::ui->progressBar->setMaximum(1);
    TimerWindow::ui->progressBar->setValue(0);
    // parse the user input
    TimerWindow::timer = new Timer(TimerWindow::ui->inputTime->text());
    TimerWindow::timer->trigger_start();
    TimerWindow::qtimer->start(1000);
    TimerWindow::rerender();
}

void TimerWindow::update_timer()
{
    if (! TimerWindow::timer->is_valid())
    {
        TimerWindow::qtimer->stop();
    }
    else
    {
        TimerWindow::timer->increment_second();
        if (TimerWindow::timer->is_finished())
        {
            TimerWindow::qtimer->stop();
        }
    }
    TimerWindow::rerender();
}

void TimerWindow::rerender()
{
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
        // update progressbar to non-running state
        TimerWindow::ui->pushToggle->setText("Start");
        TimerWindow::ui->progressBar->setMaximum(1);
        TimerWindow::ui->progressBar->setValue(0);
        TimerWindow::ui->progressBar->setFormat("");
    }
}

void TimerWindow::on_pushToggle_clicked()
{
    if (! TimerWindow::timer->matches_input(TimerWindow::ui->inputTime->text()))
    {
        // need to re-parse the user input...
        TimerWindow::timer = new Timer(TimerWindow::ui->inputTime->text());
        TimerWindow::timer->trigger_start();
        TimerWindow::qtimer->start(1000);
    }
    else
    {
        // don't need to re-parse user input, use existing timer data
        TimerWindow::timer->trigger_toggle();
        if (TimerWindow::timer->is_running())
        {
            TimerWindow::qtimer->start(1000);
        }
        else
        {
            TimerWindow::qtimer->stop();
        }
    }
    TimerWindow::rerender();
}

void TimerWindow::on_pushReset_clicked()
{
    TimerWindow::timer->trigger_reset();
    TimerWindow::qtimer->stop();
    TimerWindow::rerender();
}

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
}

TimerWindow::~TimerWindow()
{
    delete ui;
}

void TimerWindow::on_inputTime_returnPressed()
{
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
        // TODO clean the text display up
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
        }
        else
        {
            TimerWindow::ui->pushToggle->setText("Start");
        }
        TimerWindow::ui->progressBar->setMaximum(
                    TimerWindow::timer->int_total_duration()
        );
        TimerWindow::ui->progressBar->setValue(
                    TimerWindow::timer->int_remain_duration()
        );
//        QTimer *qtimer = new QTimer(this);
//        connect(qtimer, &QTimer::timeout, this, QOverload<>::of(&TimerWindow::update));
//        qtimer->start(1000);
    }
}

void TimerWindow::on_pushToggle_clicked()
{
    // TODO
    qDebug() << "pushToggle pressed... timer:is_running=" << TimerWindow::timer->is_running();
    TimerWindow::timer->trigger_toggle();
    // TODO clean the text display up
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
    }
    else
    {
        TimerWindow::ui->pushToggle->setText("Start");
    }
    TimerWindow::ui->progressBar->setMaximum(
                TimerWindow::timer->int_total_duration()
    );
    TimerWindow::ui->progressBar->setValue(
                TimerWindow::timer->int_remain_duration()
    );
}

void TimerWindow::on_pushReset_clicked()
{
    // TODO
    qDebug() << "pushReset pressed...";
    TimerWindow::timer->trigger_reset();
    // TODO clean the text display up
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
    }
    else
    {
        TimerWindow::ui->pushToggle->setText("Start");
    }
    TimerWindow::ui->progressBar->setMaximum(1);
    TimerWindow::ui->progressBar->setValue(0);
}

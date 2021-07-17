#include <QtDebug>

#include "timerwindow.h"
#include "ui_timerwindow.h"
#include "timer.h"

TimerWindow::TimerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimerWindow)
{
    TimerWindow::ui->setupUi(this);
    TimerWindow::timer = Timer();
}

TimerWindow::~TimerWindow()
{
    delete ui;
}

void TimerWindow::on_inputTime_returnPressed()
{
    qDebug() << "inputTime pressed return...";
    TimerWindow::timer = Timer(TimerWindow::ui->inputTime->text());
    if (! TimerWindow::timer.is_valid())
    {
        // TODO
        qDebug() << "input parsed invalid";
    }
    else
    {
        // TODO
        qDebug() << "input parsed valid";
    }
}


void TimerWindow::on_pushToggle_clicked()
{
    // TODO
    qDebug() << "pushToggle pressed...";
}


void TimerWindow::on_pushReset_clicked()
{
    // TODO
    qDebug() << "pushReset pressed...";
}

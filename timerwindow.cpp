#include <QtDebug>

#include "timerwindow.h"
#include "ui_timerwindow.h"

TimerWindow::TimerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimerWindow)
{
    ui->setupUi(this);
}

TimerWindow::~TimerWindow()
{
    delete ui;
}


void TimerWindow::on_inputTime_editingFinished()
{
    // TODO
    qDebug() << "editing inputTime finished...";
    qDebug() << "   raw input: " << ui->inputTime->text();
    qDebug() << "revised time: " << ui->inputTime->text();
    // << widget << "at position" << widget->pos();
}


#include <QtDebug>

#include "timerwindow.h"
#include "ui_timerwindow.h"

TimerWindow::TimerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimerWindow)
{
    ui->setupUi(this);
    qDebug() << "loaded window successfully";
}

TimerWindow::~TimerWindow()
{
    delete ui;
}


void TimerWindow::on_inputTime_editingFinished()
{
    // TODO
    qDebug() << "return pressed...";
    qDebug() << "second line test output";
    // << widget << "at position" << widget->pos();
}


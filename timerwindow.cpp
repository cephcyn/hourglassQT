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


#include "timerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerWindow w;
    w.show();
    return a.exec();
}

#include "timerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TimerWindow timer;
    timer.show();
    return app.exec();
}

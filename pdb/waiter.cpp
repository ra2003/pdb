#include "waiter.h"
//
#include <QTime>
#include <QCoreApplication>

Waiter::Waiter(QObject *parent) :
    QObject(parent)
{
}

void Waiter::wait (int i_sec)
{
    //
    QTime dieTime= QTime::currentTime().addSecs(i_sec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    };
}

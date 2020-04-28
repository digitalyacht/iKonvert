#include <QApplication>

#include "infinitecountworker_m.h"
#include "portablesleep.h"

int re = 1;

infinitecountworker_m::infinitecountworker_m()
    : m_running(true)
{

}

void infinitecountworker_m::repeat_msec(int repeatsec)
{
    re = 100 * repeatsec;
}

void infinitecountworker_m::doWork()
{
    int i = 0;
    while (m_running) {
        emit updateCount(i);
        i++;
        qApp->processEvents();
        PortableSleep::msleep(re);
    }
    emit finished();
}

void infinitecountworker_m::stopWork()
{
    m_running = false;
}

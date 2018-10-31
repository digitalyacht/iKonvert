#include <QApplication>

#include "infinitecountworker.h"
#include "portablesleep.h"

int re = 1;

InfiniteCountWorker::InfiniteCountWorker()
    : m_running(true)
{

}

void InfiniteCountWorker::repeat_sec(int repeatsec) //ms!!!!
{
    re = repeatsec;
}

void InfiniteCountWorker::doWork()
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

void InfiniteCountWorker::stopWork()
{
    m_running = false;
}

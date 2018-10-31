#ifndef __INFINITE_COUNT_WORKER_M_H__
#define __INFINITE_COUNT_WORKER_M_H__

#include <QObject>

class infinitecountworker_m : public QObject
{
    Q_OBJECT

    public:
        infinitecountworker_m();

    public slots:
        void doWork();
        void stopWork();

    private slots:
        void repeat_msec(int repeatsec);

    signals:
        void updateCount(int);
        void finished();

    private:
        bool m_running;

};

#endif // __INFINITE_COUNT_WORKER_M_H__

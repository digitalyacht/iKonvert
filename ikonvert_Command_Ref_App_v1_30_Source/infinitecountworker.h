#ifndef __INFINITE_COUNT_WORKER_H__
#define __INFINITE_COUNT_WORKER_H__

#include <QObject>

class InfiniteCountWorker : public QObject
{
    Q_OBJECT

    public:
        InfiniteCountWorker();

    public slots:
        void doWork();
        void stopWork();

    private slots:
        void repeat_sec(int repeatsec);

    signals:
        void updateCount(int);
        void finished();

    private:
        bool m_running;

};

#endif // __INFINITE_COUNT_WORKER_H__

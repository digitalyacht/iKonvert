#ifndef __COUNT_WORKER_H__
#define __COUNT_WORKER_H__

#include <QObject>

class CountWorker : public QObject
{
    Q_OBJECT

    public:
        CountWorker(int start, int end);

    public slots:
        void doWork();

    signals:
        void updateCount(int);
        void finished();

    private:
        int m_countStart;
        int m_countEnd;
};

#endif // __COUNT_WORKER_H__

#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QSettings>
#include "Driver.h"

class Worker : public QObject
{
    Q_OBJECT

    Driver *m_driver;

public:
    Worker(QSettings *settngs, QObject *parent = 0);
    ~Worker();

public slots:
    void run();
};

#endif // WORKERTHREAD_H

#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H

#include <QThread>

class Thread_Control : public QObject
{
    Q_OBJECT
public:
    explicit Thread_Control(QObject *parent = nullptr);
};

#endif // THREAD_CONTROL_H

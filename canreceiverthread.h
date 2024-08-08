#ifndef CANRECEIVERTHREAD_H
#define CANRECEIVERTHREAD_H

#include <QThread>
#include "canreceiver.h"

class CanReceiverThread : public QThread
{
    Q_OBJECT

public:
    explicit CanReceiverThread(QObject *parent = nullptr);
    ~CanReceiverThread();

    CanReceiver* getCanReceiver() const;

protected:
    void run() override;

private:
    CanReceiver *canReceiver;
};

#endif // CANRECEIVERTHREAD_H

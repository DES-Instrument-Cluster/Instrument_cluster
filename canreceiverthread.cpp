#include "canreceiverthread.h"

CanReceiverThread::CanReceiverThread(QObject *parent)
    : QThread(parent), canReceiver(new CanReceiver)
{
    canReceiver->moveToThread(this);
}

CanReceiverThread::~CanReceiverThread()
{
    delete canReceiver;
}

CanReceiver* CanReceiverThread::getCanReceiver() const
{
    return canReceiver;
}

void CanReceiverThread::run()
{
    canReceiver->startReceiving("can1");
    exec();
}

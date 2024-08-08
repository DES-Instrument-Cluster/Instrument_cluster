#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include <QObject>
#include <QCanBusDevice>
#include <QCanBusFrame>

#include "emafilter.h"

class CanReceiver : public QObject
{
    Q_OBJECT
public:
    explicit CanReceiver(QObject *parent = nullptr);
    ~CanReceiver();

    void startReceiving(const QString &interfaceName);

signals:
    void speedUpdated(double speed);

private slots:
    void processReceivedFrames();

private:
    QCanBusDevice	*canDevice;
    const int		expectedId;
    EmaFilter		emaFilter;
};

#endif // CANRECEIVER_H

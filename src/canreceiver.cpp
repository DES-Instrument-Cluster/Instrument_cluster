#include "canreceiver.h"
#include <QCanBus>
#include <QDebug>
#include <QProcess>

CanReceiver::CanReceiver(QObject *parent)
    : QObject(parent), canDevice(nullptr), expectedId(0x100), filter(new Filter(0.3, 5))
{}

CanReceiver::~CanReceiver()
{
    if (this->canDevice)
    {
        this->canDevice->disconnectDevice();
        delete this->canDevice;
    }
    delete this->filter;
}

void CanReceiver::startReceiving(const QString &interfaceName)
{
    qDebug() << "CanReceiver startReceiving()";
    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan")))
    {
        qDebug() << "device: socketcan  " << "interface: " << interfaceName;
        this->canDevice = QCanBus::instance()->createDevice("socketcan", interfaceName);
        if (this->canDevice)
        {
            qDebug() << "Successfully created CAN device";
            connect(this->canDevice, &QCanBusDevice::framesReceived, this, &CanReceiver::processReceivedFrames);
            if (!this->canDevice->connectDevice())
            {
                qDebug() << "Failed to connect CAN device";
                delete this->canDevice;
                this->canDevice = nullptr;
            }
        }
    }
}

void CanReceiver::processReceivedFrames()
{
    if (!this->canDevice)
    {
        qDebug() << "canDevice is NULL";
        return;
    }

    while (this->canDevice->framesAvailable())
    {
        qDebug() << "Message received";
        const QCanBusFrame frame = this->canDevice->readFrame();

        if (!frame.isValid())
        {
            qDebug() << "Invalid CAN frame received";
            qDebug() << "Frame: " << frame.toString();
            continue;
        }

        const int messageId = frame.frameId();
        qDebug() << "Message ID: " << messageId;

        if (messageId != this->expectedId)
        {
            continue;
        }

        QByteArray payload = frame.payload();

        if (payload.size() == sizeof(float))
        {
            unsigned int scaledSpeed = 0;
            memcpy(&scaledSpeed, payload.constData(), sizeof(unsigned int));

            float speed = this->filter->calculateOutput((float)(scaledSpeed / this->SCALE));
            if (speed < 1)
            {
                this->filter->setEma(0.0);
            }

            qDebug() << "Speed (cm/s): " << speed;

            emit speedUpdated(speed);
        }
    }
}

Filter* CanReceiver::getFilter()
{
    return this->filter;
}
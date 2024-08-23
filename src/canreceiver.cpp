#include "canreceiver.h"
#include <QCanBus>
#include <QDebug>
#include <QProcess>

CanReceiver::CanReceiver(QObject *parent, int expectedId, const QString& interfaceName)
    : QObject(parent)
    , canDevice(nullptr)
    , expectedId(expectedId)
    , interfaceName(interfaceName)
{}

CanReceiver::~CanReceiver()
{
    if (this->canDevice)
    {
        this->canDevice->disconnectDevice();
        delete this->canDevice;
    }
}

void CanReceiver::createCanDevice()
{
    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan")))
    {
        qDebug() << "device: socketcan  " << "interface: " << this->interfaceName;
        this->canDevice = QCanBus::instance()->createDevice("socketcan", this->interfaceName);
        if (this->canDevice)
        {
            qDebug() << "Successfully created CAN device";
        }
    }
}

QCanBusDevice* CanReceiver::getCanDevice() const
{
    return this->canDevice;
}

QByteArray CanReceiver::getFrontPayload()
{
    QByteArray front = this->payloadQueue.front();
    this->payloadQueue.pop();

    return front;
}

bool CanReceiver::isPayloadQueueEmpty() const
{
    return this->payloadQueue.empty();
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

        this->payloadQueue.push(frame.payload());

        // if (payload.size() == sizeof(float))
        // {
        //     unsigned int scaledSpeed = 0;
        //     memcpy(&scaledSpeed, payload.constData(), sizeof(unsigned int));

        //     float speed = this->filter->calculateOutput((float)(scaledSpeed / this->SCALE));
        //     if (speed < 1)
        //     {
        //         this->filter->setEma(0.0);
        //     }

        //     qDebug() << "Speed (cm/s): " << speed;

        //     emit speedUpdated(speed);
        // }
    }
}

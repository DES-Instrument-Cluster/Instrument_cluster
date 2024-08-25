#include "speedupdatemanager.h"

#include <QCanBusDevice>
#include <QDebug>

SpeedUpdateManager::SpeedUpdateManager(QObject *parent)
    : QObject{parent}
    , canReceiver(new CanReceiver(this, 0x100, "can1"))
    , filter(new Filter(0.3, 5))
{
    this->canReceiver->createCanDevice();
}

SpeedUpdateManager::~SpeedUpdateManager()
{
    delete this->filter;
}

void SpeedUpdateManager::start() const
{
    this->connect(this->canReceiver->getCanDevice()
              , &QCanBusDevice::framesReceived
              , this
              , &SpeedUpdateManager::processSpeedData);
    if (!this->canReceiver->getCanDevice()->connectDevice())
    {
        throw CanDeviceConnectionFailed();
    }
}

Filter* SpeedUpdateManager::getFilter()
{
    return this->filter;
}

void SpeedUpdateManager::processSpeedData()
{
    this->canReceiver->processReceivedFrames();

    while (!this->canReceiver->isPayloadQueueEmpty())
    {
        QByteArray payload = this->canReceiver->getFrontPayload();

        if (payload.size() == sizeof(uint32_t))
        {
            unsigned int scaledSpeed = 0;
            memcpy(&scaledSpeed, payload.constData(), sizeof(uint32_t));

            float speed = this->filter->calculateOutput(static_cast<float>(scaledSpeed / this->SCALE_FACTOR));
            if (speed < 1)
            {
                this->filter->setEma(0.0);
            }

            qDebug() << "Speed (cm/s): " << speed;

            emit speedUpdated(speed);
        }
    }
}

const char* SpeedUpdateManager::CanDeviceConnectionFailed::what() const throw()
{
    return "Failed to connect CAN device to slot.";
}

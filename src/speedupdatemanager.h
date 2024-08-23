#ifndef SPEEDUPDATEMANAGER_H
#define SPEEDUPDATEMANAGER_H

#include <QObject>

#include "canreceiver.h"
#include "filter.h"

class SpeedUpdateManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Filter* filter READ getFilter CONSTANT)
public:
    explicit SpeedUpdateManager(QObject *parent = nullptr);
    ~SpeedUpdateManager();

    void start() const;

    Q_INVOKABLE Filter* getFilter();

signals:
    void speedUpdated(double speed);

private:
    CanReceiver*	canReceiver;
    Filter*			filter;

    const float	SCALE_FACTOR = 10000.0;

    void processSpeedData();

    class CanDeviceConnectionFailed : public std::exception
    {
    public:
        const char* what() const throw();
    };
};

#endif // SPEEDUPDATEMANAGER_H

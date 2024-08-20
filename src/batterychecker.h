#ifndef BATTERY_CHECKER_H
#define BATTERY_CHECKER_H

#include <QProcess>
#include <QString>
#include <QDebug>
#include <QObject>

class BatteryChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int batteryLevel READ getBatteryLevel NOTIFY batteryLevelChanged)

public:
    explicit BatteryChecker(QObject *parent = nullptr);

    void monitor();

    int getBatteryLevel() const;

signals:
    void batteryLevelChanged();

private slots:
    void handleProcessStandardOutput();

private:
    QProcess	process;
    int			batteryLevel;
};

#endif // BATTERY_CHECKER_H

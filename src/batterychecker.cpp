#include "batterychecker.h"

BatteryChecker::BatteryChecker(QObject *parent) : QObject(parent)
{
    connect(&this->process, &QProcess::readyReadStandardOutput, this, &BatteryChecker::handleProcessStandardOutput);
}

void BatteryChecker::monitor()
{
    QString pythonExecutable = "venv/bin/python";
    QString scriptPath = "read_voltage.py";

    this->process.start(pythonExecutable, QStringList() << "-u" << scriptPath);
    if (!this->process.waitForStarted())
    {
        qDebug() << "Battery check process failed to start";
    }
}

int BatteryChecker::getBatteryLevel() const
{
    return this->batteryLevel;
}


void BatteryChecker::handleProcessStandardOutput()
{
    QString output = this->process.readAllStandardOutput().trimmed();
    output.remove('"');
    qDebug() << "BatteryChecker: " << output;

    float temp = output.toFloat();
    int newBatteryLevel = static_cast<int>(temp);

    if (newBatteryLevel != this->batteryLevel)
    {
        this->batteryLevel = newBatteryLevel;
        emit batteryLevelChanged();
    }
}

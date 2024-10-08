#ifndef BATTERY_CHECKER_H
#define BATTERY_CHECKER_H

#include <QObject>
#include <QTimer>
#include "ina219reader.h"
#include "./filter/smafilter.h"

// Класс BatteryChecker предназначен для мониторинга параметров батареи через I2C.
// The BatteryChecker class is designed to monitor battery parameters via I2C.

class BatteryChecker : public QObject
{
    Q_OBJECT
public:
    explicit BatteryChecker(QObject *parent = nullptr);
    ~BatteryChecker();

    void startMonitoring(int interval = DEFAULT_INTERVAL_MS); // Метод для запуска мониторинга с заданным интервалом
    // Method to start monitoring with a specified interval

    float getBusVoltage() const;
    float getShuntVoltage() const;
    float getAmper() const;
    Q_INVOKABLE bool getIsCharging() const;
    Q_INVOKABLE int getBusVoltagePercent() const;

signals:
    void chargingStateUpdated(bool isCharging); // Сигнал, который вызывается при изменении состояния зарядки
    // Signal emitted when the charging state changes
    void batteryPercentageUpdated(int busVoltagePercent);

private slots:
    void readBatteryData(); // Слот для чтения данных с батареи
    // Slot for reading battery data

private:
    void checkIsCharge(); // Метод для проверки изменения состояния зарядки
    // Method to check for changes in the charging state
    void updateBusVoltagePercent(); // Метод для обновления процента напряжения на шине
    // Method to update the bus voltage percentage

    INA219Reader *ina219Reader; // Указатель на объект для чтения данных через I2C
    // Pointer to the object for reading data via I2C
    SmaFilter *voltageFilter; // Указатель на фильтр для сглаживания значений напряжения
    // Pointer to the filter for smoothing voltage values

    float busVoltage; // Переменная для хранения текущего значения напряжения на шине
    // Variable for storing the current bus voltage
    float shuntVoltage; // Переменная для хранения текущего значения напряжения на шунте
    // Variable for storing the current shunt voltage
    float amper; // Переменная для хранения текущего значения ампеража
    // Variable for storing the current amperage
    bool isCharging; // Переменная для хранения состояния зарядки
    // Variable for storing the charging state
    int busVoltagePercent; // Переменная для хранения процента напряжения на шине
    // Variable for storing the bus voltage percentage
    float voltageRange; // Переменная для хранения разницы между максимальным и минимальным напряжением на шине
    // Variable for storing the difference between max and min bus voltage

    QTimer *timer; // Таймер для периодического опроса данных
    // Timer for periodic data polling

    static constexpr float MIN_BUS_VOLTAGE = 9.65f; // Минимальное допустимое напряжение на шине (вольты)
    // Minimum allowable bus voltage (volts)
    static constexpr float MAX_BUS_VOLTAGE = 12.5f; // Максимальное допустимое напряжение на шине (вольты)
    // Maximum allowable bus voltage (volts)
    static constexpr float CHARGE_VOLTAGE_INCREMENT = 0.88f; // Прибавка к напряжению на шине при зарядке (вольты)
    // Voltage increment on the bus during charging (volts)
    static constexpr int DEFAULT_INTERVAL_MS = 1000; // Интервал опроса по умолчанию в миллисекундах
    // Default polling interval in milliseconds
    static constexpr int FILTER_WINDOW_SIZE = 10; // Размер окна фильтрации
    // Filter window size

    // Константы для определения состояния зарядки
    // Constants for determining the charging state
    // Константы для определения состояния зарядки
    // Constants for determining the charging state
    static constexpr int SHUNT_VOLTAGE_MIN_THRESHOLD = 40;  // Минимальное пороговое значение напряжения на шунте (милливольты)
    // Minimum shunt voltage threshold (millivolts)
    static constexpr int SHUNT_VOLTAGE_MAX_THRESHOLD = 400; // Максимальное пороговое значение напряжения на шунте (милливольты)
    // Maximum shunt voltage threshold (millivolts)
    static constexpr int AMPERAGE_MIN_THRESHOLD = 3;  // Минимальное пороговое значение силы тока (миллиамперы)
    // Minimum amperage threshold (milliamps)
    static constexpr int AMPERAGE_MAX_THRESHOLD = 60; // Максимальное пороговое значение силы тока (миллиамперы)
    // Maximum amperage threshold (milliamps)
};

#endif // BATTERY_CHECKER_H

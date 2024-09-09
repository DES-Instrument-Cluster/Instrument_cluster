#include "batterychecker.h"
#include <QDebug>

BatteryChecker::BatteryChecker(QObject *parent)
    : QObject(parent)
    , busVoltage(0)
    , shuntVoltage(0)
    , amper(0)
    , isCharging(false)
    , busVoltagePercent(0)
{
    this->ina219Reader = new INA219Reader(); // Инициализация объекта для чтения данных через I2C
    // Initializing the object for reading data via I2C
    this->voltageFilter = new SmaFilter(this, FILTER_WINDOW_SIZE); // Инициализация фильтра с заданным окном
    // Initializing the filter with the specified window size

    this->voltageRange = MAX_BUS_VOLTAGE - MIN_BUS_VOLTAGE; // Вычисляем разницу между макс. и мин. напряжением на шине
    // Calculate the difference between max and min bus voltage

    this->timer = new QTimer(this); // Создаем таймер для периодического чтения данных
    // Create a timer for periodic data reading
    connect(this->timer, &QTimer::timeout, this, &BatteryChecker::readBatteryData); // Подключаем таймер к слоту чтения данных
    // Connect the timer to the slot for reading data
}

BatteryChecker::~BatteryChecker() {
    timer->stop(); // Останавливаем таймер перед уничтожением объекта
    // Stop the timer before the object is destroyed
    delete ina219Reader; // Удаляем объект для чтения данных через I2C
    // Delete the I2C reader object
    delete voltageFilter; // Удаляем объект фильтра
    // Delete the filter object
}

void BatteryChecker::startMonitoring(int interval) {
    timer->start(interval); // Запускаем таймер с заданным интервалом
    // Start the timer with the specified interval
}

float BatteryChecker::getBusVoltage() const {
    return this->busVoltage; // Возвращаем текущее значение напряжения на шине
    // Return the current bus voltage
}

float BatteryChecker::getShuntVoltage() const {
    return this->shuntVoltage; // Возвращаем текущее значение напряжения на шунте
    // Return the current shunt voltage
}

float BatteryChecker::getAmper() const {
    return this->amper; // Возвращаем текущее значение ампеража
    // Return the current amperage
}

bool BatteryChecker::getIsCharging() const {
    return this->isCharging; // Возвращаем текущее состояние зарядки
    // Return the current charging state
}

int BatteryChecker::getBusVoltagePercent() const {
    return busVoltagePercent; // Возвращаем процентное значение напряжения на шине
    // Return the bus voltage percentage
}

void BatteryChecker::readBatteryData() {
    try {
        this->busVoltage = ina219Reader->readBusVoltage(); // Чтение текущего значения напряжения на шине // Read the current bus voltage
        this->shuntVoltage = ina219Reader->readShuntVoltage(); // Чтение текущего значения напряжения на шунте
        // Read the current shunt voltage
        this->amper = ina219Reader->readAmper(); // Чтение текущего значения ампеража
        // Read the current amperage

        this->checkIsCharge(); // Проверка изменения состояния зарядки
        // Check for changes in the charging state
        this->updateBusVoltagePercent(); // Обновление процентного значения напряжения на шине
        // Update the bus voltage percentage
    } catch (const std::exception &e) {
        qCritical() << "Exception: " << e.what();
    }
}

void BatteryChecker::checkIsCharge() {
    // Если напряжение на шунте больше 40 мВ, но меньше 400 мВ, и сила тока больше 3 мА, но меньше 60 мА, то батарея заряжается.
    // If the shunt voltage is greater than 40 mV but less than 400 mV, and the current is greater than 3 mA but less than 60 mA, then the battery is charging.
    bool new_isCharge = this->shuntVoltage > SHUNT_VOLTAGE_MIN_THRESHOLD
                        && this->shuntVoltage < SHUNT_VOLTAGE_MAX_THRESHOLD
                        && this->amper > AMPERAGE_MIN_THRESHOLD
                        && this->amper < AMPERAGE_MAX_THRESHOLD;
    // Проверка условия зарядки с учетом новых пороговых значений
    // Check the charging condition considering the new threshold values

    if (new_isCharge != this->isCharging) { // Если состояние зарядки изменилось, обновляем и отправляем сигнал
        // If the charging state has changed, update and emit a signal
        this->isCharging = new_isCharge;
        emit chargingStateUpdated(this->isCharging);
    }
}

void BatteryChecker::updateBusVoltagePercent() {
    float adjustedBusVoltage = this->busVoltage; // Начальное значение напряжения на шине
    // Initial bus voltage value

    if (this->isCharging) { // Если идет зарядка, уменьшаем напряжение на шине на значение инкремента
        // If charging, reduce the bus voltage by the increment value
        adjustedBusVoltage -= CHARGE_VOLTAGE_INCREMENT;
    }

    float relativeVoltage = (adjustedBusVoltage - MIN_BUS_VOLTAGE) / this->voltageRange; // Рассчитываем относительное напряжение
    // Calculate the relative voltage
    int newBusVoltagePercent = static_cast<int>(relativeVoltage * 100.0f); // Переводим в проценты
    // Convert to percentage

    if (newBusVoltagePercent < 0) {
        newBusVoltagePercent = 0;
    } else if (newBusVoltagePercent > 100) {
        newBusVoltagePercent = 100;
    }

    this->busVoltagePercent = static_cast<int>(this->voltageFilter->calculateFilteredOutput(static_cast<double>(newBusVoltagePercent))); // Применяем фильтр для сглаживания
    // Apply the filter to smooth the output
    emit batteryPercentageUpdated(this->busVoltagePercent);
}

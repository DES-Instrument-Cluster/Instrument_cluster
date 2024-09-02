#include "ina219reader.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

INA219Reader::INA219Reader()
{
	this->openI2cBus();
	this->setI2cDeviceAddress();
}

INA219Reader::~INA219Reader()
{
	close(this->fd);
}

float INA219Reader::readBusVoltage() const
{
	this->writeRegisterValueToI2cDevice(BUS_VOLTAGE_REGISTER);
	int busVoltageRaw = this->readRegisterValueFromI2cDevice();
	float busVoltage = (busVoltageRaw >> 3) * 0.004f; // V
	return busVoltage;
}

float INA219Reader::readShuntVoltage() const
{
    this->writeRegisterValueToI2cDevice(SHUNT_VOLTAGE_REGISTER);
    int16_t shuntVoltageRaw = this->readRegisterValueFromI2cDevice();
    float shuntVoltage = shuntVoltageRaw * 0.01f; // mV
    return shuntVoltage;
}

void INA219Reader::openI2cBus()
{
	if ((this->fd = open(I2C_BUS, O_RDWR)) < 0) {
		throw I2cBusOpenFailException();
	}
}

void INA219Reader::setI2cDeviceAddress() const
{
	if (ioctl(this->fd, I2C_SLAVE, INA219_ADDRESS) < 0) {
		close(this->fd);
		throw SetI2cDeviceAddressFailException();
	}
}

void INA219Reader::writeRegisterValueToI2cDevice(unsigned char reg) const
{
	if (write(this->fd, &reg, 1) != 1) {
		throw WriteRegisterValueToI2cDeviceFailException();
	}
}

int16_t INA219Reader::readRegisterValueFromI2cDevice() const
{
	unsigned char buffer[2];

	if (read(this->fd, buffer, 2) != 2) {
		throw ReadRegisterValueFromI2cDeviceFailException();
	}
    return static_cast<int16_t>((buffer[0] << 8) | buffer[1]);
}

const char* INA219Reader::I2cBusOpenFailException::what() const throw()
{
	return "Failed to open the I2C bus";
}

const char* INA219Reader::SetI2cDeviceAddressFailException::what() const throw()
{
	return "Failed to acquire bus access and/or talk to slave.";
}

const char* INA219Reader::WriteRegisterValueToI2cDeviceFailException::what() const throw()
{
	return "Failed to write to I2C register.";
}

const char* INA219Reader::ReadRegisterValueFromI2cDeviceFailException::what() const throw()
{
	return "Failed to read from I2C register.";
}

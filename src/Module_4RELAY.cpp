#include "Module_4RELAY.h"

/*! @brief Initialize the 4RELAY.*/
bool MODULE_4RELAY::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl,
                          uint32_t speed) {
    _wire = wire;
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    _wire->begin((int)_sda, (int)_scl, speed);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

/*! @brief Write a certain length of data to the specified register address.
    @return True if the write was successful, otherwise false.. */
bool MODULE_4RELAY::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                               uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->write(buffer, length);
    if (_wire->endTransmission() == 0) return true;
    return false;
}

/*! @brief Read a certain length of data to the specified register address.
    @return True if the read was successful, otherwise false.. */
bool MODULE_4RELAY::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                              uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

/*! @brief Read all relay status. */
uint8_t MODULE_4RELAY::getAllRelayState() {
    uint8_t data = 0;
    readBytes(_addr, MODULE_4RELAY_CTL_REG, &data, 1);
    return data;
}

/*! @brief Read the status of the specified relay. */
bool MODULE_4RELAY::getRelayState(uint8_t index) {
    uint8_t stateByte = getAllRelayState();
    return (stateByte >> index) & 0x01;
}

/*! @brief Set the state of the specified relay. */
bool MODULE_4RELAY::setRelay(uint8_t index, bool state) {
    uint8_t stateByte = getAllRelayState();
    if (state) {
        stateByte |= (0x01 << index);
    } else {
        stateByte &= ~(0x01 << index);
    }
    return writeBytes(_addr, MODULE_4RELAY_CTL_REG, &stateByte, 1);
}

/*! @brief Set all relay states. */
bool MODULE_4RELAY::setAllRelay(bool state) {
    uint8_t data = (state ? 0xff : 0x00);
    return writeBytes(_addr, MODULE_4RELAY_CTL_REG, &data, 1);
}

/*! @brief Read the color of led lights.
    @return True if the read was successful, otherwise false.. */
uint16_t MODULE_4RELAY::getRawADC(adc_resolution_t resolution) {
    uint8_t data[2] = {0};
    uint16_t rawADC = 0;
    if (resolution == ADC_8BIT) {
        if (readBytes(_addr, MODULE_4RELAY_8BIT_ADC_REG, data, 1)) {
            rawADC = data[0];
        } else {
            return 0;
        };
    } else {
        if (readBytes(_addr, MODULE_4RELAY_12BIT_ADC_REG, data, 2)) {
            rawADC = data[1];
            rawADC = (rawADC << 8) | data[0];
        } else {
            return 0;
        };
    }
    return rawADC;
}

float MODULE_4RELAY::getVoltage() {
    uint16_t rawADC = getRawADC(ADC_12BIT);
    float voltage   = (float)rawADC / 4095.0 * 3.3 * 8.0;
    return voltage;
}

bool MODULE_4RELAY::setDeviceAddr(uint8_t addr) {
    uint8_t data = addr;
    if (writeBytes(_addr, MODULE_4RELAY_I2C_ADDR_REG, &data, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}

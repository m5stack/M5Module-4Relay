#include "MODULE_4RELAY.h"

bool MODULE_4RELAY::begin(TwoWire* wire, uint8_t addr, uint8_t sda, uint8_t scl,
                          uint32_t speed) {
    _wire = wire;
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    _wire->begin(_sda, _scl);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

bool MODULE_4RELAY::writeReg(uint8_t data) {
    _wire->beginTransmission(_addr);
    _wire->write(MODULE_4RELAY_REG);
    _wire->write(data);
    if (_wire->endTransmission() == 0) return true;
    return false;
}

uint8_t MODULE_4RELAY::readReg() {
    _wire->beginTransmission(_addr);
    _wire->write(MODULE_4RELAY_REG);
    _wire->endTransmission(false);
    _wire->requestFrom(_addr, 1);
    return _wire->read();
}

uint8_t MODULE_4RELAY::getAllRelayState() {
    return readReg();
}

bool MODULE_4RELAY::getRelayState(uint8_t index) {
    uint8_t stateByte = getAllRelayState();
    return (stateByte >> index) & 0x01;
}

bool MODULE_4RELAY::setRelay(uint8_t index, bool state) {
    uint8_t stateByte = getAllRelayState();
    if (state) {
        stateByte |= (0x01 << index);
    } else {
        stateByte &= ~(0x01 << index);
    }
    return writeReg(stateByte);
}

bool MODULE_4RELAY::setAllRelay(bool state) {
    return writeReg(state ? 0xff : 0x00);
}

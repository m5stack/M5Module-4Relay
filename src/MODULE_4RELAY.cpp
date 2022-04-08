#include "MODULE_4RELAY.h"

void MODULE_4RELAY::begin(TwoWire* wire, uint8_t addr, uint8_t sda, uint8_t scl,
                          uint32_t speed) {
    _wire  = wire;
    _addr  = addr;
    _sda   = sda;
    _scl   = scl;
    _speed = speed;
    _wire->begin(_sda, _scl, _speed);
}

void MODULE_4RELAY::writeReg(uint8_t data) {
    _wire->beginTransmission(_addr);
    _wire->write(MODULE_4RELAY_REG);
    _wire->write(data);
    _wire->endTransmission();
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

void MODULE_4RELAY::setRelay(uint8_t index, bool state) {
    uint8_t stateByte = getAllRelayState();
    if (state) {
        stateByte |= (0x01 << index);
    } else {
        stateByte &= ~(0x01 << index);
    }
    writeReg(stateByte);
}

void MODULE_4RELAY::setAllRelay(bool state) {
    writeReg(state ? 0xff : 0x00);
}

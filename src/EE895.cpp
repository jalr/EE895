#include "EE895.h"

EE895::EE895() {
}

bool EE895::begin(TwoWire &twoWirePort) {
  port = &twoWirePort;
  #if defined(ARDUINO_ARCH_ESP8266)
  port->setClockStretchLimit(200000);
  #endif

  return getSensorName().compareTo(EE895_DEVICE_NAME) == 0;
}

void EE895::setDebug(Stream &debugStream) {
  debug = &debugStream;
}

void EE895::debugEndTransmission(byte status) {
  if (debug == NULL) {
    return;
  }
  switch (status) {
    case 1:
      debug->println(F("data too long to fit in transmit buffer"));
      break;
    case 2:
      debug->println(F("received NACK on transmit of address"));
      break;
    case 3:
      debug->println(F("received NACK on transmit of data "));
      break;
    default:
      debug->println(F("other error"));
      break;
  }
}

uint8_t* EE895::readRegister(uint16_t startingAddress, uint16_t noOfRegisters) {
  ModbusTransmitFrame tFrame = ModbusTransmitFrame(*port);
  tFrame.beginTransmission(EE895_ADDRESS_MODBUS);
  tFrame.write((uint8_t)EE895_FUNCTION_READ);
  tFrame.write(startingAddress);
  tFrame.write(noOfRegisters);
  uint8_t transmitFrameResult = tFrame.endTransmission();

  if (transmitFrameResult != 0) {
    debugEndTransmission(transmitFrameResult);
    return NULL;
  }

  ModbusReceiveFrame rFrame = ModbusReceiveFrame(*port);
  rFrame.requestFrom((int)EE895_ADDRESS_MODBUS, (int)(4 + (2 * noOfRegisters)));

  if (rFrame.readByte() != EE895_FUNCTION_READ) {
    if (debug != NULL) {
      debug->println(F("received unexpected function code"));
    }
    return NULL;
  }

  uint8_t payloadSize = rFrame.readByte();
  if (payloadSize != (2 * noOfRegisters)) {
    if (debug != NULL) {
      debug->println(F("received unexpected byte count"));
    }
    return NULL;
  }

  uint8_t *payload = new uint8_t[payloadSize];

  rFrame.read(payload, payloadSize);

  if (!rFrame.end()) {
    if (debug != NULL) {
      debug->println(F("Checksum error"));
    }
    return NULL;
  }

  return payload;
}

bool EE895::writeSingleRegister(uint16_t address, uint16_t value) {
  ModbusTransmitFrame tFrame = ModbusTransmitFrame(*port);
  tFrame.beginTransmission(EE895_ADDRESS_MODBUS);
  tFrame.write((uint8_t)EE895_FUNCTION_WRITE);
  tFrame.write(address);
  tFrame.write(value);
  uint8_t transmitFrameResult = tFrame.endTransmission();

  if (transmitFrameResult != 0) {
    debugEndTransmission(transmitFrameResult);
    return false;
  }

  ModbusReceiveFrame rFrame = ModbusReceiveFrame(*port);
  rFrame.requestFrom((int)EE895_ADDRESS_MODBUS, (int)(7));

  if (rFrame.readByte() != EE895_FUNCTION_WRITE) {
    if (debug != NULL) {
      debug->println(F("received unexpected function code"));
    }
    return false;
  }

  uint16_t readBackAddress = rFrame.readWord();

  if (readBackAddress != address) {
    if (debug != NULL) {
      debug->println(F("Didn't read back the same address as transmitted"));
    }
    return false;
  }

  uint16_t readBackValue = rFrame.readWord();

  if (readBackValue != value) {
    if (debug != NULL) {
      debug->println(F("Didn't read back the same value as transmitted"));
    }
    return false;
  }

  if (!rFrame.end()) {
    if (debug != NULL) {
      debug->println(F("Checksum error"));
    }
    return false;
  }

  return true;
}

float EE895::readRegisterFloat(uint16_t address) {
  union {
    uint8_t b[4];
    float f;
  } value;

  uint8_t *registerContents = readRegister(address, 2);
  if (registerContents == NULL) {
    return NAN;
  }

  value.b[1] = registerContents[0];
  value.b[0] = registerContents[1];
  value.b[3] = registerContents[2];
  value.b[2] = registerContents[3];

  return value.f;
}

uint16_t EE895::readRegisterUInt(uint16_t address) {
  uint8_t *reg = readRegister(address, 1);
  return (reg[0] << 8) | reg[1];
}

int16_t EE895::readRegisterInt(uint16_t address) {
  uint8_t *reg = readRegister(address, 1);
  return (reg[0] << 8) | reg[1];
}

String EE895::getSerialNumber() {
  char* serialNumber = (char*)readRegister(EE895_REGISTER_NAME, 8);
  if (serialNumber) {
    return String(serialNumber);
  } else {
    return String("");
  }
}

String EE895::getSensorName() {
  char* sensorName = (char*)readRegister(EE895_REGISTER_SERIAL, 8);
  if (sensorName) {
    return String(sensorName);
  } else {
    return String("");
  }
}

uint8_t EE895::getFirmwareVersionMajor() {
  return readRegister(EE895_REGISTER_FIRMWARE_VERSION, 1)[0];
}

uint8_t EE895::getFirmwareVersionMinor() {
  return readRegister(EE895_REGISTER_FIRMWARE_VERSION, 1)[1];
}

uint8_t EE895::getMeasuringMode() {
  return readRegister(EE895_REGISTER_MEASURING_MODE, 1)[1] & 1;
}

bool EE895::isDataReady() {
  return readRegister(EE895_REGISTER_MEASURING_STATUS, 1)[1] & (1 << 0);
}

bool EE895::isReadyForTrigger() {
  return readRegister(EE895_REGISTER_MEASURING_STATUS, 1)[1] & (1 << 1);
}

uint16_t EE895::getCO2MeasuringInterval() {
  return readRegisterUInt(EE895_REGISTER_PARAMETER_MEASURING_INTERVAL);
};

uint16_t EE895::getFilterCoefficient() {
  return readRegisterUInt(EE895_REGISTER_PARAMETER_FILTER_COEFFICIENT);
};

int16_t EE895::getCO2CustomerOffset() {
  return readRegisterInt(EE895_REGISTER_PARAMETER_CUSTOMER_OFFSET);
}

bool EE895::setCO2MeasuringInterval(uint16_t value) {
  return writeSingleRegister(EE895_REGISTER_PARAMETER_MEASURING_INTERVAL, value);
}

bool EE895::setFilterCoefficient(uint16_t value) {
  return writeSingleRegister(EE895_REGISTER_PARAMETER_FILTER_COEFFICIENT, value);
}

bool EE895::setCO2CustomerOffset(int16_t value) {
  return writeSingleRegister(EE895_REGISTER_PARAMETER_CUSTOMER_OFFSET, value);
}

#include "EE895.h"

EE895::EE895(){
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

uint16_t EE895::updateCRC(uint16_t data, uint16_t crc) {
  return EE895::updateCRC((uint8_t)data, EE895::updateCRC((uint8_t)(data >> 8), crc));
}

uint16_t EE895::updateCRC(uint8_t data, uint16_t crc) {
  crc ^= data;
  for (uint8_t bit = 8; bit > 0; bit--) {
    if ((crc & 0x0001) != 0) {
      crc >>= 1;
      crc ^= 0xA001;
    }
    else
      crc >>= 1;
  }
  return crc;
}

byte EE895::transmitFrame(uint8_t functionCode, uint16_t startingAdress, uint16_t noOfRegisters) {
  port->beginTransmission(EE895_ADDRESS_MODBUS);
  uint16_t crc = updateCRC((uint8_t)EE895_ADDRESS_MODBUS);

  port->write(functionCode);
  crc = updateCRC(functionCode, crc);

  port->write(startingAdress >> 8);
  port->write(startingAdress & 0xff);
  crc = updateCRC(startingAdress, crc);

  port->write(noOfRegisters >> 8);
  port->write(noOfRegisters & 0xff);
  crc = updateCRC(noOfRegisters, crc);

  port->write(crc & 0xff);
  port->write(crc >> 8);
  return port->endTransmission();
}


uint8_t* EE895::readRegister(uint16_t startingAdress, uint16_t noOfRegisters) {
  byte transmitFrameResult = transmitFrame(EE895_FUNCTION_READ, startingAdress, noOfRegisters);
  if (transmitFrameResult != 0) {
    if (debug != NULL) {
      debug->print(F("Error transmitting Frame: "));
      switch (transmitFrameResult) {
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
    return NULL;
  }

  const uint8_t receivedBytesCount = port->requestFrom((int)EE895_ADDRESS_MODBUS, (int)(4 + (2*noOfRegisters)), (int)true);
  uint8_t *payload = new uint8_t[2*noOfRegisters];
  uint16_t crcCalculated = updateCRC((uint8_t)(EE895_ADDRESS_MODBUS));
  uint16_t crcReceived = 0;

  for(uint8_t i = 0; i < receivedBytesCount; i++) {
    if (!port->available()) {
      if (debug != NULL) {
        debug->println(F("received less bytes than expected"));
      }
      return NULL;
    }

    uint8_t receivedByte = port->read();

    if ((i == 0) && (receivedByte != EE895_FUNCTION_READ)) {
      if (debug != NULL) {
        debug->println(F("received unexpected function code"));
      }
      return NULL;
    }
    else if ((i == 1) && (receivedByte != (2*noOfRegisters))) {
      if (debug != NULL) {
        debug->println(F("received unexpected byte count"));
      }
      return NULL;
    }
    else if (i < (2*noOfRegisters + 2)) {
      crcCalculated = updateCRC(receivedByte, crcCalculated);
      payload[i-2] = receivedByte;
    }
    else if (i == (2*noOfRegisters + 2)) {
      crcReceived = receivedByte;
    }
    else if (i == (2*noOfRegisters + 3)) {
      crcReceived |= receivedByte<<8;
    }
  }

  if (crcCalculated != crcReceived) {
    if (debug != NULL) {
      debug->println(F("Checksum error"));
    }
    return NULL;
  }

  return payload;
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
  return readRegister(EE895_REGISTER_MEASURING_MODE, 1)[0] & 1;
}

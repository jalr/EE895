#include "EE895.h"

EE895::EE895(){
}

bool EE895::begin(TwoWire &twoWirePort) {
  port = &twoWirePort;
#if defined(ARDUINO_ARCH_ESP8266)
  port->setClockStretchLimit(200000);
#endif
  port->begin();
  return true;
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

bool EE895::send(uint8_t functionCode, uint16_t startingAdress, uint16_t noOfRegisters) {
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
  return (port->endTransmission() == 0);
}


bool EE895::readRegister(uint16_t startingAdress, uint16_t noOfRegisters) {
  return send(EE895_FUNCTION_READ, startingAdress, noOfRegisters);
}

float EE895::readRegisterFloat(uint16_t address) {
  union {
    uint8_t b[4];
    float f;
  } value;

  if (!readRegister(address, 0x0002))
    // no ACK
    return NAN;

  const uint8_t receivedBytesCount = port->requestFrom((uint8_t)EE895_ADDRESS_MODBUS, (size_t)8, true);

  uint16_t crcCalculated = updateCRC((uint8_t)(EE895_ADDRESS_MODBUS));
  uint16_t crcReceived = 0;
  for(uint8_t i = 0; i < receivedBytesCount; i++) {
    if (!port->available()) {
      // no bytes available
      return NAN;
    }
    uint8_t response = port->read();
    if (i < 6) {
      crcCalculated = updateCRC(response, crcCalculated);
    }
    switch(i) {
      case 0:
        if (response != EE895_FUNCTION_READ) {
          return NAN;
        }
        break;
      case 1:
        if (response != 4) { // we expect 4 bytes
          return NAN;
        }
        break;
      case 2: // register 1 Hi
        value.b[1] = response;
        break;
      case 3: // register 1 Lo
        value.b[0] = response;
        break;
      case 4: // register 2 Hi
        value.b[3] = response;
        break;
      case 5: // register 2 Lo
        value.b[2] = response;
        break;
      case 6:
        crcReceived = response;
        break;
      case 7:
        crcReceived |= response<<8;
        break;
    }
  }

  if (crcCalculated != crcReceived) {
    return NAN;
  }

  return value.f;
}

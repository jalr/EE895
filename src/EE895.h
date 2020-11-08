#ifndef __EE895_ARDUINO_LIBARARY_H__
#define __EE895_ARDUINO_LIBARARY_H__

#include "Arduino.h"
#include <Wire.h>

#define EE895_ADDRESS_MODBUS 0x5F
#define EE895_FUNCTION_READ  0x03
#define EE895_FUNCTION_WRITE 0x06


class EE895 {
  public:
    EE895(void);

    bool begin(TwoWire &twoWirePort = Wire);

    bool send(uint8_t functionCode, uint16_t startingAdress, uint16_t noOfRegisters);

    bool readRegister(uint16_t startingAdress, uint16_t noOfRegisters);

    float readRegisterFloat(uint16_t address);

    float getCO2Average() { return readRegisterFloat(0x0424); };
    uint16_t updateCRC(uint8_t data, uint16_t crc = 0xFFFF);
    uint16_t updateCRC(uint16_t data, uint16_t crc = 0xFFFF);

  private:
    TwoWire *port;
};

#endif /* !__EE895_ARDUINO_LIBARARY_H__ */

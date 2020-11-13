#ifndef __EE895_ARDUINO_LIBARARY_H__
#define __EE895_ARDUINO_LIBARARY_H__

#include "Arduino.h"
#include <Wire.h>

#define EE895_DEVICE_NAME "EE895"

#define EE895_ADDRESS_MODBUS 0x5F
#define EE895_FUNCTION_READ  0x03
#define EE895_FUNCTION_WRITE 0x06

#define EE895_REGISTER_NAME              0x0000
#define EE895_REGISTER_FIRMWARE_VERSION  0x0008
#define EE895_REGISTER_SERIAL            0x0009
#define EE895_REGISTER_TEMPERATURE_DEG_C 0x03EA
#define EE895_REGISTER_TEMPERATURE_DEG_F 0x03EC
#define EE895_REGISTER_TEMPERATURE_K     0x03F0
#define EE895_REGISTER_CO2_RAW           0x0426
#define EE895_REGISTER_PRESSURE_MBAR     0x04B0
#define EE895_REGISTER_PRESSURE_PSI      0x04B2


class EE895 {
  public:
    EE895();

    bool begin(TwoWire &twoWirePort = Wire);

    void setDebug(Stream &debugStream);

    float getCO2Average() { return readRegisterFloat(0x0424); };

    float getTemperatureInDegreeC() { return readRegisterFloat(EE895_REGISTER_TEMPERATURE_DEG_C); };
    float getTemperatureInDegreeF() { return readRegisterFloat(EE895_REGISTER_TEMPERATURE_DEG_F); };
    float getTemperatureInK() { return readRegisterFloat(EE895_REGISTER_TEMPERATURE_K); };
    float getCO2Raw() { return readRegisterFloat(EE895_REGISTER_CO2_RAW); };
    float getPressureInmbar() { return readRegisterFloat(EE895_REGISTER_PRESSURE_MBAR); };
    float getPressureInpsi() { return readRegisterFloat(EE895_REGISTER_PRESSURE_PSI); };

    uint8_t getFirmwareVersionMajor();
    uint8_t getFirmwareVersionMinor();

    String getSerialNumber();
    String getSensorName();

  private:
    byte transmitFrame(uint8_t functionCode, uint16_t startingAdress, uint16_t noOfRegisters);
    uint8_t* readRegister(uint16_t startingAdress, uint16_t noOfRegisters);
    float readRegisterFloat(uint16_t address);
    uint16_t updateCRC(uint8_t data, uint16_t crc = 0xFFFF);
    uint16_t updateCRC(uint16_t data, uint16_t crc = 0xFFFF);
    TwoWire *port;
    Stream *debug;
};

#endif /* !__EE895_ARDUINO_LIBARARY_H__ */

#ifndef EE895_H
#define EE895_H

#include "Arduino.h"
#include <Wire.h>
#include "ModbusTransmitFrame.h"
#include "ModbusReceiveFrame.h"

#define EE895_DEVICE_NAME "EE895"

#define EE895_ADDRESS_MODBUS 0x5F
#define EE895_FUNCTION_READ  0x03
#define EE895_FUNCTION_WRITE 0x06

#define EE895_REGISTER_NAME                         0x0000
#define EE895_REGISTER_FIRMWARE_VERSION             0x0008
#define EE895_REGISTER_SERIAL                       0x0009
#define EE895_REGISTER_MEASURING_MODE               0x01F8
#define EE895_REGISTER_MEASURING_STATUS             0x01F9
#define EE895_REGISTER_MEASURING_TRIGGER            0x01FA
#define EE895_REGISTER_DETAILED_STATUS              0x0258
#define EE895_REGISTER_TEMPERATURE_DEG_C            0x03EA
#define EE895_REGISTER_TEMPERATURE_DEG_F            0x03EC
#define EE895_REGISTER_TEMPERATURE_K                0x03F0
#define EE895_REGISTER_CO2_RAW                      0x0426
#define EE895_REGISTER_PRESSURE_MBAR                0x04B0
#define EE895_REGISTER_PRESSURE_PSI                 0x04B2
#define EE895_REGISTER_PARAMETER_MEASURING_INTERVAL 0x1450
#define EE895_REGISTER_PARAMETER_FILTER_COEFFICIENT 0x1451
#define EE895_REGISTER_PARAMETER_CUSTOMER_OFFSET    0x1452

class EE895 {
  public:
    typedef enum {
      CO2TooHigh = (1 << 0),
      CO2TooLow = (1 << 1),
      TemperatureTooHigh = (1 << 2),
      TemperatureTooLow = (1 << 3),
      PressureTooHigh = (1 << 6),
      PressureTooLow = (1 << 7)
    } DetailedStatus;

    EE895();

    bool begin(TwoWire &twoWirePort = Wire);

    void setDebug(Stream &debugStream);

    float getCO2Average() {
      return readRegisterFloat(0x0424);
    };

    float getTemperatureInDegreeC() {
      return readRegisterFloat(EE895_REGISTER_TEMPERATURE_DEG_C);
    };
    float getTemperatureInDegreeF() {
      return readRegisterFloat(EE895_REGISTER_TEMPERATURE_DEG_F);
    };
    float getTemperatureInK() {
      return readRegisterFloat(EE895_REGISTER_TEMPERATURE_K);
    };
    float getCO2Raw() {
      return readRegisterFloat(EE895_REGISTER_CO2_RAW);
    };
    float getPressureInmbar() {
      return readRegisterFloat(EE895_REGISTER_PRESSURE_MBAR);
    };
    float getPressureInpsi() {
      return readRegisterFloat(EE895_REGISTER_PRESSURE_PSI);
    };

    uint8_t getFirmwareVersionMajor();
    uint8_t getFirmwareVersionMinor();

    uint8_t getMeasuringMode();
    bool setMeasuringModeSingleShot() {
      return writeSingleRegister(EE895_REGISTER_MEASURING_MODE, 1);
    };
    bool setMeasuringModeContinous() {
      return writeSingleRegister(EE895_REGISTER_MEASURING_MODE, 0);
    };

    String getSerialNumber();
    String getSensorName();

    bool isDataReady();
    bool isReadyForTrigger();

    bool triggerMeasurement() {
      return writeSingleRegister(EE895_REGISTER_MEASURING_TRIGGER, 1);
    }

    uint16_t getDetailedStatus();

    uint16_t getCO2MeasuringInterval();
    uint16_t getFilterCoefficient();
    int16_t getCO2CustomerOffset();

    bool setCO2MeasuringInterval(uint16_t value);
    bool setFilterCoefficient(uint16_t value);
    bool setCO2CustomerOffset(int16_t value);


  private:
    uint8_t* readRegister(uint16_t startingAddress, uint16_t noOfRegisters);
    bool writeSingleRegister(uint16_t address, uint16_t value);
    float readRegisterFloat(uint16_t address);
    uint16_t readRegisterUInt(uint16_t address);
    int16_t readRegisterInt(uint16_t address);
    void debugEndTransmission(byte status);
    TwoWire *port;
    Stream *debug;
};

#endif /* !EE895_H */

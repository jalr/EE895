#ifndef MODBUS_TRANSMIT_FRAME_H
#define MODBUS_TRANSMIT_FRAME_H

#include "ModbusFrame.h"

class ModbusTransmitFrame : public ModbusFrame {
  public:
    ModbusTransmitFrame(TwoWire &twoWirePort)
      : ModbusFrame{twoWirePort} {
    }
    void write(uint8_t data);
    void write(uint16_t data);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission();
};

#endif /* !MODBUS_TRANSMIT_FRAME_H */

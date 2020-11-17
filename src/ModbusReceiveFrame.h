#ifndef MODBUS_RECEIVE_FRAME_H
#define MODBUS_RECEIVE_FRAME_H

#include "ModbusFrame.h"

class ModbusReceiveFrame : public ModbusFrame {
  public:
    ModbusReceiveFrame(TwoWire &twoWirePort)
      : ModbusFrame{twoWirePort} {
    }
    uint8_t readByte();
    uint16_t readWord();
    void read(uint8_t *buffer, uint8_t size);
    uint8_t requestFrom(int address, int size);
    bool end();
};

#endif /* !MODBUS_RECEIVE_FRAME_H */

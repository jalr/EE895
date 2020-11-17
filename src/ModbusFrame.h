#ifndef MODBUS_FRAME_H
#define MODBUS_FRAME_H

#include <Wire.h>

class ModbusFrame {
  public:
    ModbusFrame(TwoWire &twoWirePort);
    uint16_t getCRC();
    bool compareCRC(uint8_t high, uint8_t low);

  protected:
    TwoWire *port;
    void updateCRC(uint8_t data);
    void updateCRC(uint16_t data);
    uint16_t crc = 0xFFFF;
};

#endif /* !MODBUS_FRAME_H */

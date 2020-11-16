#include "ModbusFrame.h"

ModbusFrame::ModbusFrame(TwoWire &twoWirePort) {
  port = &twoWirePort;
}

void ModbusFrame::updateCRC(uint8_t data) {
  crc ^= data;
  for (uint8_t bit = 8; bit > 0; bit--) {
    if ((crc & 0x0001) != 0) {
      crc >>= 1;
      crc ^= 0xA001;
    } else {
      crc >>= 1;
    }
  }
}

void ModbusFrame::updateCRC(uint16_t data) {
  updateCRC((uint8_t)(data >> 8));
  updateCRC((uint8_t)data);
}

uint16_t ModbusFrame::getCRC() {
  return crc;
}

bool ModbusFrame::compareCRC(uint8_t high, uint8_t low) {
  return (crc >> 8) == high && (crc & 0xff) == low;
}

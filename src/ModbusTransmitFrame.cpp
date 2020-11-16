#include "ModbusTransmitFrame.h"

void ModbusTransmitFrame::write(uint8_t data) {
  port->write(data);
  updateCRC(data);
}

void ModbusTransmitFrame::write(uint16_t data) {
  port->write(data >> 8);
  port->write(data & 0xff);
  updateCRC(data);
}

void ModbusTransmitFrame::beginTransmission(uint8_t address) {
  port->beginTransmission(address);
  updateCRC(address);
}

uint8_t ModbusTransmitFrame::endTransmission() {
  port->write(crc & 0xff);
  port->write(crc >> 8);
  return port->endTransmission();
}

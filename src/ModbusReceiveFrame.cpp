#include "ModbusReceiveFrame.h"

uint8_t ModbusReceiveFrame::readByte() {
  uint8_t received = port->read();
  updateCRC(received);
  return received;
}

uint16_t ModbusReceiveFrame::readWord() {
  uint16_t received = port->read() << 8;
  received |= port->read();
  updateCRC(received);
  return received;
}

void ModbusReceiveFrame::read(uint8_t* buffer, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    buffer[i] = readByte();
  }
}

uint8_t ModbusReceiveFrame::requestFrom(int address, int size) {
  updateCRC((uint8_t)address);
  uint8_t receivedBytesCount = port->requestFrom(address, size, (int)true);
  return receivedBytesCount;
}

bool ModbusReceiveFrame::end() {
  uint8_t crcLow = port->read();
  uint8_t crcHigh = port->read();
  return compareCRC(crcHigh, crcLow);
}

#include <Arduino.h>
#include <EE895.h> // https://github.com/jalr/EE895

EE895 sensor = EE895();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\r\n\r\nEE895 Demo");
  Serial.print("SDA is on Pin:");
  Serial.print(SDA);
  Serial.print(", SCL is on Pin:");
  Serial.println(SCL);

  Wire.begin();

  if (!sensor.begin()) {
    Serial.println("Sensor not found, please check connections.");
  }

  sensor.setDebug(Serial); // this will print errors on serial port
}

void loop() {
  float co2Average = sensor.getCO2Average();

  Serial.print("CO2 average: ");

  printDetailedStatus();

  Serial.println();

  delay(5000);
}

void printDetailedStatus() {
  uint16_t status = sensor.getDetailedStatus();
  if (status & EE895::DetailedStatus::CO2TooHigh) {
    Serial.println("CO 2 measurement too high");
  } else if (status & EE895::DetailedStatus::CO2TooLow) {
    Serial.println("CO 2 measurement too low");
  }
  if (status & EE895::DetailedStatus::TemperatureTooHigh) {
    Serial.println("T measurement too high");
  } else if (status & EE895::DetailedStatus::TemperatureTooLow) {
    Serial.println("T measurement too low");
  }
  if (status & EE895::DetailedStatus::PressureTooHigh) {
    Serial.println("p measurement too high");
  } else if (status & EE895::DetailedStatus::PressureTooLow) {
    Serial.println("p measurement too low");
  }
}

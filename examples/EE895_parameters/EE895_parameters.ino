#include <Arduino.h>
#include <EE895.h> // https://github.com/jalr/EE895

EE895 sensor = EE895();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\r\n\r\nEE895 Parameters");

  Wire.begin();

  if (!sensor.begin()) {
    Serial.println("Sensor not found, please check connections.");
  }
}

void loop() {
  Serial.println();
  Serial.println("menu Selection");
  Serial.println("1) show parameters");
  Serial.println("2) restore defaults");

  while (Serial.available() == 0) {
    yield();
  }

  int incomingByte = Serial.read();
  switch (incomingByte) {
    case '1':
      Serial.print("getCO2MeasuringInterval: ");
      Serial.println(sensor.getCO2MeasuringInterval());

      Serial.print("getFilterCoefficient: ");
      Serial.println(sensor.getFilterCoefficient());

      Serial.print("getCO2CustomerOffset: ");
      Serial.println(sensor.getCO2CustomerOffset());
      break;
    case '2':
      Serial.println("restoring defaults");

      // The CO2 parameter settings are non-volatile since they are stored in the flash memory.
      sensor.setCO2MeasuringInterval(150); // scale: 1:10, 150 => 15s
      sensor.setFilterCoefficient(4);
      sensor.setCO2CustomerOffset(0);
      break;
  }
}

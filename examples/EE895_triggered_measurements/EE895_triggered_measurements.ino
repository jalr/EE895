#include <Arduino.h>
#include <EE895.h> // https://github.com/jalr/EE895

EE895 sensor = EE895();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\r\n\r\nEE895 triggered measurements Demo");

  Wire.begin();

  if (!sensor.begin()) {
    Serial.println("Sensor not found, please check connections.");
  }

  sensor.setDebug(Serial);

  if (sensor.setMeasuringModeSingleShot()) {
    Serial.println("Successfully switched measuring mode to single shot.");
  } else {
    Serial.println("Error switching measuring mode");
  }
}

void loop() {
  if (sensor.isDataReady()) {
    float co2Average = sensor.getCO2Average();
    Serial.print("CO2 average: ");
    Serial.println(co2Average);
  }

  Serial.print("Waiting for sensor to be ready for a new measurement cycle.");
  while (!sensor.isReadyForTrigger()) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  /* The minimum time interval between two triggers is 10s.

     For accurate measurement results,
     there are recommended six single shot measurements
     under stable temperature conditions.
  */
  if (sensor.triggerMeasurement()) {
    Serial.println("Measurement successfully triggered.");
  } else {
    Serial.println("Error triggering measurement");
  }

  Serial.print("Waiting until data is available for read out.");
  while (!sensor.isDataReady()) {
    Serial.print(".");
    delay(20);
  }
  Serial.println();
}

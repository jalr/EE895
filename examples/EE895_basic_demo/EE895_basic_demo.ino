#include <Arduino.h>
#include <EE895.h>

EE895 sensor = EE895();

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\r\n\r\nEE895 Demo");
  Serial.print("SDA is on Pin:");
  Serial.print(SDA);
  Serial.print(", SCL is on Pin:");
  Serial.println(SCL);

  sensor.begin();
}

void loop() {
  float co2Average = sensor.getCO2Average();

  Serial.print("CO2 average: ");
  Serial.println(co2Average);
  delay(5000);
}

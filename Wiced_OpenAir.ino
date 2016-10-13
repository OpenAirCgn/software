#include "OpenAirSensor.h"

OpenAirSensor sensor(A0,A1,A2,A3); // Feather M0 Wifi
//OpenAirSensor sensor(C3,C2,A3,A2); // Wiced Feather

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  Serial.println(sensor.NO2_ppm());
  delay(1000);
}

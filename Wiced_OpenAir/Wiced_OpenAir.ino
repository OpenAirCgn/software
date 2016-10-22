#include "OpenAirSensor/OpenAirSensor.h"
// config lines - if you are using it on feather M0/Wiced
#ifdef ARDUINO_STM32_FEATHER 
  #define en WAKE //or PA0
  #define pre PC3
  #define vnox PA3
  #define vred PC2
  #define in_type INPUT_ANALOG
  #define resolution 4096
#elif defined(ARDUINO_SAMD_FEATHER_M0)
  #define en A0
  #define pre A1
  #define vred A2
  #define vnox A3
  #define in_type INPUT
  #define resolution 1024
#endif
  
OpenAirSensor sensor(en,pre,vred,vnox);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(vnox, in_type);
  pinMode(vred, in_type);
  sensor.change_resolution(resolution); //change to your current resolution steps of the ADC;

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  delay(10000);
  digitalWrite(pre, 1);
  delay(30000);
  digitalWrite(pre, 0);
  
  Serial.println(sensor.NO2_ppm());
  Serial.println(sensor.CO_ppm());
}

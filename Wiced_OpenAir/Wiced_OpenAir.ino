#include "OpenAirSensor/OpenAirSensor.h"
// config lines - if you are using it on feather M0/Wiced
#ifdef ARDUINO_STM32_FEATHER 
  #define en PC3
  #define pre PC2
  #define vred PA3
  #define vnox PA2
  #define in_type INPUT_ANALOG
#elif defined(ARDUINO_SAMD_FEATHER_M0)
  #define en A0
  #define pre A1
  #define vred A2
  #define vnox A3
  #define in_type INPUT
#endif
  
OpenAirSensor sensor(en,pre,vred,vnox);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(vnox, in_type);
  pinMode(vred, in_type);
  //sensor.change_resolution(4096); change to your current resolution steps of the ADC;

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  Serial.println(sensor.NO2_ppm());
  Serial.println(sensor.r_ox); // get the calculated resistance
  delay(1000);
}

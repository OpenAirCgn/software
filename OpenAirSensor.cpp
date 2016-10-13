/*
  OpenAirSensor.cpp - Library for NO2 and CO Sensor reading.
  Created by Marcel Belledin, October 12, 2016.
  Released into the public domain.
*/

#include <Arduino.h>
#include "OpenAirSensor.h"

OpenAirSensor::OpenAirSensor(int pre,int en,int vred,int vnox)
{
  pinMode(pre, OUTPUT);
  _pre = pre;
  pinMode(en, OUTPUT);
  _en = en;
  pinMode(vred, INPUT);
  _vred = vred;
  pinMode(vnox, INPUT);
  _vnox = vnox;
}

/*********************************************************************************************************
** Descriptions:            power on sensor
*********************************************************************************************************/
void OpenAirSensor::powerOn(void)
{
  digitalWrite(_en, HIGH);
}

/*********************************************************************************************************
** Descriptions:            power off sensor
*********************************************************************************************************/
void OpenAirSensor::powerOff(void)
{
  digitalWrite(_en, LOW);
}
/*********************************************************************************************************
** Descriptions:            power on heater
*********************************************************************************************************/
void OpenAirSensor::heaterOn(void)
{
  digitalWrite(_pre, HIGH);
}

/*********************************************************************************************************
** Descriptions:            power off heater
*********************************************************************************************************/
void OpenAirSensor::heaterOff(void)
{
  digitalWrite(_pre, LOW);
}

/*********************************************************************************************************
** Descriptions:            change defoult sampling rate
*********************************************************************************************************/
void OpenAirSensor::changeSamplerate(int rate)
{
  samplerate = rate;
}

/*********************************************************************************************************
** Descriptions:            read out pin and calculate resistance 
*********************************************************************************************************/
float OpenAirSensor::get_ox_resistane(void)
{
  vout_ox = (3.3 / samplerate) * analogRead(_vnox); // Calculates the Voltage
  r_ox = ((vin - vout_ox) * r5)/vout_ox; // Calculates the resistance

  return r_ox;
}

/*********************************************************************************************************
** Descriptions:            doCalibration on average resistance of NO t.b.d. in clean air
*********************************************************************************************************/
void OpenAirSensor::doCalibrate(void)
{
  int sum_ox = 0;
  for(int j = 0; j < 20; j++) {
    sum_ox += get_ox_resistane();
    delay(100);
  }
  r0_ox = sum_ox / 20;
}

/*********************************************************************************************************
** Descriptions:            measure NO2
* Links:
*********************************************************************************************************/
float OpenAirSensor::NO2_ppm(void)
{
  ratio_ox = get_ox_resistane() / r0_ox;
  ppm_ox = pow(ratio_ox, -1.67)/1.47; // Calculates ppm
  return isnan(ppm_ox)?-3:ppm_ox;
}


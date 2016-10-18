/*
  OpenAirSensor.cpp - Library for NO2 and CO Sensor reading.
  Created by Marcel Belledin, October 12, 2016.
  Released into the public domain.
*/

#include <Arduino.h>
#include "OpenAirSensor.h"

OpenAirSensor::OpenAirSensor(int en,int pre,int vred,int vnox)
{
  pinMode(pre, OUTPUT);
  _pre = pre;
  pinMode(en, OUTPUT);
  _en = en;
  _vred = vred;
  _vnox = vnox;
}

/*********************************************************************************************************
** Descriptions:            power on sensor
*********************************************************************************************************/
void OpenAirSensor::power_on(void)
{
  digitalWrite(_en, HIGH);
}

/*********************************************************************************************************
** Descriptions:            power off sensor
*********************************************************************************************************/
void OpenAirSensor::power_off(void)
{
  digitalWrite(_en, LOW);
}
/*********************************************************************************************************
** Descriptions:            power on heater
*********************************************************************************************************/
void OpenAirSensor::heater_on(void)
{
  digitalWrite(_pre, HIGH);
}

/*********************************************************************************************************
** Descriptions:            power off heater
*********************************************************************************************************/
void OpenAirSensor::heater_off(void)
{
  digitalWrite(_pre, LOW);
}

/*********************************************************************************************************
** Descriptions:            change defoult resolution of the ADC
*********************************************************************************************************/
void OpenAirSensor::change_resolution(int res)
{
  resolution = res;
}

/*********************************************************************************************************
** Descriptions:            read out pin and calculate resistance 
*********************************************************************************************************/
float OpenAirSensor::get_ox_resistane(void)
{
  vout_ox = (board_volt / resolution) * analogRead(_vnox); // Calculates the Voltage
  r_ox = ((board_volt - vout_ox) * r5)/vout_ox; // Calculates the resistance

  return r_ox;
}

/*********************************************************************************************************
** Descriptions:            read out pin and calculate resistance 
*********************************************************************************************************/
float OpenAirSensor::get_co_resistane(void)
{
  vout_co = (board_volt / resolution) * analogRead(_vnox); // Calculates the Voltage
  r_co = ((board_volt - vout_co) * r7)/vout_co; // Calculates the resistance

  return r_co;
}

/*********************************************************************************************************
** Descriptions:            doCalibration on average resistance of NO t.b.d. in clean air
*********************************************************************************************************/
void OpenAirSensor::do_calibrate(void)
{
  int sum_ox = 0;
  for(int j = 0; j < 20; j++) {
    sum_ox += get_ox_resistane();
    delay(100);
  }
  r0_ox = sum_ox / 20;

  int sum_co = 0;
  for(int j = 0; j < 20; j++) {
    sum_co += get_co_resistane();
    delay(100);
  }
  r0_co = sum_co / 20;
}

/*********************************************************************************************************
** Descriptions:            measure NO2
* Links:
*********************************************************************************************************/
float OpenAirSensor::NO2_ppm(void)
{
  ratio_ox = get_ox_resistane() / r0_ox;
  ppm_ox = pow(ratio_ox, 1.007)/6.855; // Calculates ppm
  return isnan(ppm_ox)?-3:ppm_ox;
}

/*********************************************************************************************************
** Descriptions:            measure CO
* Links:
*********************************************************************************************************/
float OpenAirSensor::CO_ppm(void)
{
  ratio_co = get_co_resistane() / r0_co;
  ppm_co = pow(ratio_co, -1.179)*4.385; // Calculates ppm
  return isnan(ppm_co)?-3:ppm_co;
}



/*
  OpenAirSensor.h - Library for NO2 and CO Sensor reading.
  Created by Marcel Belledin, October 12, 2016.
  Released into the public domain.
*/
#ifndef OpenAirSensor_h
#define OpenAirSensor_h

class OpenAirSensor
{
  public:
    OpenAirSensor(int pre,int en,int vnox,int vred);
    void power_on(void);
    void power_off(void);
    void heater_on(void);
    void heater_off(void);
    void change_resolution(int);
    void do_calibrate(void);
    float CO_ppm();
    float NO2_ppm();

    float r0_ox = 900; // assumed resistance in fresh air... to be calibrated
    float r0_co = 200; // assumed resistance in fresh air... to be calibrated
    float r_ox = 0; // calculated resistance
    float r_co = 0; // calculated resistance       
     

  private:
    int _pre;
    int _en;
    int _vnox;
    int _vred;

    // vars for resistor calculation
    int resolution = 1023; // resolution steps of the ADC
    float board_volt; // input voltage
    float vout_ox = 0; // output voltage
    float vout_co = 0; // output voltage
    float r5 = 3900; // RLOAD_OX in Ohm
    float r7 = 360000; // RLOAD_CO in Ohm

    // vars for ppm calculation
    float ratio_ox = 0;
    float ppm_ox = 0;
    float ratio_co = 0;
    float ppm_co = 0;
    
    float get_ox_resistane(void);
    float get_co_resistane(void);
};

#endif

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
    void powerOn(void);
    void powerOff(void);
    void heaterOn(void);
    void heaterOff(void);
    void changeSamplerate(int);
    void doCalibrate(void);
    float CO_ppm();
    float NO2_ppm();

    float r0_ox = 900; // assumed resistance in fresh air... to be calibrated

  private:
    int _pre;
    int _en;
    int _vnox;
    int _vred;

    // vars for resistor calculation
    int vin = 5; // input voltage
    int samplerate = 1024;
    float vout_ox = 0; // output voltage
    float r5 = 3900; // RLOAD_OX in Ohm
    float r_ox = 0; // calculated resistance       
    
    // vars for ppm calculation
    float ratio_ox = 0;
    float ppm_ox = 0;

    float get_ox_resistane(void);
};

#endif

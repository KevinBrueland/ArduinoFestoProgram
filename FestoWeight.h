#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h>

#ifndef FestoWeight_h
#define FestoWeight_h
#include "Arduino.h"
#include "DataContainer.h"

class FestoWeight
{
  public:
    FestoWeight(Stream *serial, byte analogReadPin, DataContainer *dataContainer);
    float WeighJar();
    
    
    
  private:
    void SetAnalogReferenceTo2V56();
    void SetAnalogReferenceToDefault();
    double ReadSignalValue();
    double ConvertSignalToMiliVolts(double signalValue);
    double ConvertMiliVoltsToGrams(double miliVoltValue);
    FilterOnePole _lowpassFilter;
    DataContainer *_dataContainer;

    byte _analogReadPin;
    int _adcResolution = 1024;
    int _refVoltage;
    float _weightScalingConstant = 0.003723; //to be calcualted
    int _measurementsToAverage = 1000;
    int _filterFrequency = 50;

    Stream *_serial;

  
};

#endif

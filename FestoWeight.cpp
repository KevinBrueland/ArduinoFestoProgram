#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h>

#include "Arduino.h"
#include "FestoWeight.h"
#include "DataContainer.h"

FestoWeight::FestoWeight(Stream *serial, byte analogReadPin, DataContainer *dataContainer)
{
  _serial = serial;
  _analogReadPin = analogReadPin;
  SetAnalogReferenceTo2V56();
  FilterOnePole lowpassFilter(LOWPASS, _filterFrequency);
  _lowpassFilter = lowpassFilter;
}

void FestoWeight::SetAnalogReferenceTo2V56()
{
  analogReference(INTERNAL2V56);
  _refVoltage = 2560;
  
}

void FestoWeight::SetAnalogReferenceToDefault()
{
  analogReference(DEFAULT);
  _refVoltage = 5000;
}

float FestoWeight::WeighJar()
{
  delay(200);
  float signalValue = ReadSignalValue();

  float miliVoltValue = ConvertSignalToMiliVolts(signalValue);

  float weightInGrams = ConvertMiliVoltsToGrams(miliVoltValue);
  _serial->print("Weight: ");
  _serial->print(weightInGrams);
  _serial->println(" grams");

  _dataContainer->measuredWeight = weightInGrams;
  return weightInGrams;
}

double FestoWeight::ReadSignalValue()
{
  double averageSignalValue = 0;
  for(int i = 0; i <= _measurementsToAverage; i++)
  {
    averageSignalValue += _lowpassFilter.input(analogRead(_analogReadPin));
    delay(1); //allowing analog input to stabilize between readings
  }
  
  double signalValue = (averageSignalValue/_measurementsToAverage);
  
//  _serial->print("Signal value: ");
//  _serial->println(signalValue);
  
  return signalValue;
}

double FestoWeight::ConvertSignalToMiliVolts(double signalValue)
{
  double miliVoltValue =  (signalValue/_adcResolution) * _refVoltage;
  
//  _serial->print("miliVolt Value: ");
//  _serial->println(miliVoltValue);
  
  return miliVoltValue;
}

double FestoWeight::ConvertMiliVoltsToGrams(double miliVoltValue)
{
  double weightInGrams =  (miliVoltValue * 0.03556841718) - 1.18087145;
//  _serial->print("weight: ");
//  _serial->print(weightInGrams);  Commented out until the weightScalingConstant has been calculated
//  _serial->println(" grams");

  return weightInGrams;
}


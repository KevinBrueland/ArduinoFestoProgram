#include "Arduino.h"
#include "JarValidator.h"


JarValidator::JarValidator(Stream *serial)
{
  _serial = serial;
}

void JarValidator::WeighJar()
{
  //get some value from the weight, here just a dummy variable atm
  float signalValue = 1374;
  _jarWeight = ConvertSignalToWeight(signalValue);
  _serial->println("Jar weight: " + (String)_jarWeight);
}

float JarValidator::ConvertSignalToWeight(float signalValue)
{
  float weight = signalValue * _weightScalingFactor;

  return weight;
}

void JarValidator::SetJarWeight(float jarWeight)
{
  _jarWeight = jarWeight;
}

void JarValidator::SetOrderWeight(float orderWeight)
{
  _orderWeight = orderWeight;
}

void JarValidator::SetTearWeight(float tearWeight)
{
  _tearWeight = tearWeight;
}

bool JarValidator::CompareJarWeightWithOrderWeight()
{
  _serial->println("Comparing product weight in current jar with order weight");
  float productWeight = _jarWeight - _tearWeight;
  _serial->println("product weight: " + (String)productWeight);
  float deviation = abs(_orderWeight - productWeight);
  _serial->println("Devation of current jar weight: " + (String)deviation + " grams");
  
  float allowedDeviation = CalculateAllowedDeviaton();
  _serial->println("Checking: " + (String)deviation + " < " + (String)allowedDeviation);
  if(deviation < allowedDeviation)
  {
    _serial->println("Deviation within acceptable bounds. Jar accepted");
    return true;    
  }
  else
  {
    _serial->println("Deviaton outside acceptable bounds. Jar discarded");
    return false;
  }
}

float JarValidator::CalculateAllowedDeviaton()
{  
  float allowedDeviaton = _orderWeight*_allowedConfidenceInterval;
  _serial->println("Allowed deviaton: " + (String)allowedDeviaton + " grams");
  
  return abs(allowedDeviaton);
 
}


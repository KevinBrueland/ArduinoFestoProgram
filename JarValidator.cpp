#include "Arduino.h"
#include "JarValidator.h"
#include "dataContainer.h"


JarValidator::JarValidator(Stream *serial, DataContainer *dataContainer)
{
  _serial = serial;
  dataContainer = _dataContainer;
}


bool JarValidator::CompareJarWeightWithOrderWeight()
{
  if(_dataContainer->jarSize == 1)
  {
    :tareWeight = 14.54;
    //_tareWeight = 14.68933333; 
    _allowedDeviation = 1.0; //replace with actual value
  }
  else if(_dataContainer->jarSize == 2)
  {
    _tareWeight = 23.54;
    //_tareWeight = 23.72666667; 
    _allowedDeviation = 2.0; //replace with actual value
  }
  
  _serial->println("Comparing product weight in current jar with order weight");
  float productWeight = _dataContainer->measuredWeight - _tareWeight;
  _serial->println("product weight: " + (String)productWeight);
  
  float deviation = abs(_dataContainer->orderWeight - productWeight);
  _serial->println("Devation of current jar: " + (String)deviation + " grams");
  _dataContainer->measuredWeight = productWeight;
  _serial->println("Checking: " + (String)deviation + " < " + (String)_allowedDeviation);
  if(deviation < _allowedDeviation)
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



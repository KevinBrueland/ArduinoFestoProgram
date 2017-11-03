#ifndef FestoJarValidator_h;
#define FestoJarValidator_h;
#include "Arduino.h";
#include "DataContainer.h"


class JarValidator
{
  
  public:
    JarValidator(Stream *serial, DataContainer *dataContainer);
    bool CompareJarWeightWithOrderWeight();  
    
  private:
   Stream *_serial;
   DataContainer *_dataContainer;  
   
   float _allowedDeviation;;
   float _tearWeight; 
  
};

#endif

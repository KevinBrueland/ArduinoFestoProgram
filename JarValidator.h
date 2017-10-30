#ifndef FestoJarValidator_h;
#define FestoJarValidator_h;
#include "Arduino.h";
#include "RFIDDataContainer.h"


class JarValidator
{
  
  public:
    JarValidator(Stream *serial);
    bool CompareJarWeightWithOrderWeight();
    
    void WeighJar();   
    float ConvertSignalToWeight(float signalValue);
    
    void SetJarWeight(float jarWeight);
    void SetTearWeight(float tearWeight);
    void SetOrderWeight(float orderWeight); 
    
    
  private:
   Stream *_serial;
   
   float _jarWeight;
   float _allowedDeviaton;   
   float _tearWeight;
   float _orderWeight;
   
   const float _weightScalingFactor = 0.0372;
   const float _allowedConfidenceInterval = 0.05;
   
   float CalculateAllowedDeviaton();
  
 
  
  
  
};

#endif

#ifndef HexDecConverter_h
#define HexDecConverter_h

#include "Arduino.h"

class HexDecConverter
{
  public:
    int HexToDecimal(String hexString);
    String DecimalToHex(byte decimalValue, byte desiredStringLength);
    
};

#endif


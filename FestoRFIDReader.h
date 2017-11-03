#ifndef FestoRFIDReader_h
#define FestoRFIDReader_h
#include "Arduino.h"
#include "DataContainer.h"

class FestoRFIDReader
{
  public:
    FestoRFIDReader(Stream *rs232Serial, Stream *serial, DataContainer *dataContainer);
    bool ReadRFID();
    void RequestValueFromRFID(char address);
    int ReadOrderId();

  private:
   // void RequestValueFromRFID(char address);
    int ParseDataToInt(String data);
    String ParseDataToString(String data);
    float ParseDataToFloat(String data);
    //int ReadOrderId();
    int ReadItemId();
    int ReadJarSize();
    float ReadOrderWeight();
    
    DataContainer *_dataContainer;
    Stream *_serial;
    Stream *_rs232Serial;

    char _orderIdTagAddress = '4';
    char _itemIdTagAddress = '5';
    char _itemSizeTagAddress = '6';
    char _orderWeightTagAddress = '7';
    char _dataEndCharacter = '#';
    
};

#endif

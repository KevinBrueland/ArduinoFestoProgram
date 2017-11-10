#ifndef FestoRFIDReader_h
#define FestoRFIDReader_h
#include "Arduino.h"
#include "DataContainer.h"

class FestoRFIDReader
{
  public:
    FestoRFIDReader(Stream *rs232Serial, Stream *serial, DataContainer *dataContainer);
    bool ReadRFID();

  private:
    void RequestData();
    void SetDataCarrierType();
    
    DataContainer *_dataContainer;
    Stream *_serial;
    Stream *_rs232Serial;
    
};

#endif

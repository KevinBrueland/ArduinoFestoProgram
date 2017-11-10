#include "FestoRFIDReader.h"
#include "Arduino.h"
#include "DataContainer.h"
#include "HexDecConverter.h"

FestoRFIDReader::FestoRFIDReader(Stream *rs232Serial, Stream *serial, DataContainer *dataContainer)
{ 
  _rs232Serial = rs232Serial;
  _serial = serial;
  _dataContainer = dataContainer;
}

bool FestoRFIDReader::ReadRFID()
{
  char data[8];
  int index = 0;
   SetDataCarrierType();
   _serial->println("Reading values from RFID");
   char cmd[] = {'S', 'R', '1', '0', '0', '0', '0', '0', '2', '#', '\r', '\0'};
   
   _rs232Serial->print(cmd);

   delay(500);
   
   while(_rs232Serial->available())
    {
      delay(100);
      char c = _rs232Serial->read();
      data[index] = c;
      index++;
    }

    _dataContainer->orderID = data[2];
    _serial->print("Order ID: ");
    _serial->println(_dataContainer->orderID, DEC);
    delay(100);
    _dataContainer->itemID = data[3];
    _serial->print("Item ID: ");
    _serial->println(_dataContainer->itemID, DEC);
    delay(100);
    _dataContainer->jarSize = data[4];
    _serial->print("Jar Size: ");
    _serial->println(_dataContainer->jarSize, DEC);
    delay(100);
    _dataContainer->orderWeight = data[5];
    _serial->print("Order Weight: ");
    _serial->println(_dataContainer->orderWeight, DEC);
    delay(100);
    _serial->println();
    _serial->println("RFID Reading Complete");
}



void FestoRFIDReader::SetDataCarrierType()
{
   char set[] = {'C', 'T', '1', '0', '3', '#', '\r', '\0'};
   _rs232Serial->print(set); //setting the data carrier type to IPC03 
}

void FestoRFIDReader::RequestData()
{
  char cmd[] = {'S', 'R', '1', '0', '0', '0', '0' ,'0', '2', '#', '\r', '\0'};
  _rs232Serial->print(cmd);
  delay(500);
}


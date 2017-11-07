#include "FestoRFIDReader.h"
#include "Arduino.h"
#include "DataContainer.h"

FestoRFIDReader::FestoRFIDReader(Stream *rs232Serial, Stream *serial, DataContainer *dataContainer)
{ 
  _rs232Serial = rs232Serial;
  _serial = serial;
  _dataContainer = dataContainer;
}

bool FestoRFIDReader::ReadRFID()
{
   _serial->println("Reading values from RFID");
   
   _dataContainer->orderID = ReadOrderId();
   _serial->print("Order ID: ");
   _serial->println(_dataContainer->orderID);
   delay(50);
   _dataContainer->itemID = ReadItemId();
   _serial->print("Item ID: ");
   _serial->println(_dataContainer->itemID);
   delay(50);
   _dataContainer->jarSize = ReadJarSize();
   _serial->print("Jar Size: ");
   _serial->println(_dataContainer->jarSize);
   delay(50);
   _dataContainer->orderWeight = ReadOrderWeight();
   _serial->print("Order Weight: ");
   _serial->println(_dataContainer->orderWeight);

   _serial->println("RFID Reading Complete");
   
}

void FestoRFIDReader::RequestValueFromRFID(char address)
{
    char cmd[] = {'S', 'R', '1', '0', '0', '0', address, '0', '1', '#', '\r', '\0'};

    _rs232Serial->print(cmd);
    //_serial->print(cmd);
    delay(1000);
}

int FestoRFIDReader::ReadOrderId()
{
    String data;
    RequestValueFromRFID(_orderIdTagAddress);
    
    while(_rs232Serial->available())
    {
      delay(100);
      char c = _rs232Serial->read();
      data += c;
    }
    //_serial->println(data);
    
    int ParsedData = ParseDataToInt(data);

    return ParsedData;
    

}

int FestoRFIDReader::ReadItemId()
{
   String data;
   RequestValueFromRFID(_itemIdTagAddress);

    while(_rs232Serial->available())
    {
      delay(1);
      char c = _rs232Serial->read();
      data += c;
    }

    int ParsedData = ParseDataToInt(data);

    return ParsedData;
}


int FestoRFIDReader::ReadJarSize()
{
   String data;
   RequestValueFromRFID(_itemSizeTagAddress); 

   while(_rs232Serial->available())
   {
    delay(1);
      char c = _rs232Serial->read();
      data += c;
   }

    int ParsedData = ParseDataToInt(data);

    return ParsedData;

}

float FestoRFIDReader::ReadOrderWeight()
{
   String data;
   RequestValueFromRFID(_orderWeightTagAddress);

   while(_rs232Serial->available())
   {
      delay(1);
      char c = _rs232Serial->read();
      data += c;
   }

    float ParsedData = ParseDataToFloat(data);

    return ParsedData;
}

int FestoRFIDReader::ParseDataToInt(String data)
{
  int endDelim = data.indexOf(_dataEndCharacter); // end character in rfid data received

  String parsedData = data.substring(2, endDelim);

  int parsedValue = parsedData.toInt();

  return parsedValue;
}

String FestoRFIDReader::ParseDataToString(String data)
{
  int endDelim = data.indexOf(_dataEndCharacter);

  String parsedData = data.substring(2, endDelim);

  return parsedData;
}

float FestoRFIDReader::ParseDataToFloat(String data)
{
  int endDelim = data.indexOf(_dataEndCharacter);

  String parsedData = data.substring(2, endDelim);

  float parsedValue = parsedData.toFloat();

  return parsedValue;
}


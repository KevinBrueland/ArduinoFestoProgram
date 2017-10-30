#include <ArduinoJson.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include "HttpService.h"
#include "CommonDefs.h"
#include "RFIDDataContainer.h"
#include "JarValidator.h"

//constants
const int baudRate = 9600;
const byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0xC0, 0x28};
const byte apiServer[] = {52,178,208,12}; //api server address
String apiHost = "festorest.azurewebsites.net";
String itemTrackerResource = "/api/itemtrackers";
bool isRFIDRead = false;

//Pins - to be replaced with actual pin numbers
byte pinRequestedToReadRFID = 2;
byte pinReadingRFIDComplete = 3;
byte pinRequestedToReadWeight = 5;
byte pinReadingWeightComplete = 6;
byte pinReportToRobotWeightOK = 7;

//Instantiating our objects
IPAddress staticDeviceIp(192,168,1,136);
EthernetClient client;
HttpService *httpService;
JarValidator jarValidator(&Serial);
RFIDDataContainer tagDataContainer;



void ConfigurePins()
{
  //inputs
  pinMode(pinRequestedToReadRFID, INPUT);
  pinMode(pinRequestedToReadWeight, INPUT);

  //outputs
  pinMode(pinReadingRFIDComplete, OUTPUT);
  pinMode(pinReadingWeightComplete, OUTPUT);
  pinMode(pinReportToRobotWeightOK, OUTPUT);
}

void AttemptToEstablishEthernetConnection()
{
  Serial.begin(baudRate);
  Serial.println("Requesting IP from DHCP server...");
  if(Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to connect to DHCP server");
    Serial.println("Using provided static ip address:");
    Serial.println(staticDeviceIp);
    Ethernet.begin(mac, staticDeviceIp);
  }
  
  Serial.println("IP provided by DHCP"); 
  Serial.println("Device IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  Serial.println("Ethernet connection established");

  //delay(1000);
}


void setup() 
{
  httpService = new HttpService(&Serial, &client, apiServer, apiHost);
  ConfigurePins();  
  AttemptToEstablishEthernetConnection();
  String itemTrackerToPost = httpService->FormatItemTrackerPostData(7,20,FAILED);
      httpService->Post(itemTrackerResource,itemTrackerToPost);    
}


void loop() 
{
//  if(RobotRequestingRFIDRead())
//  {
//    digitalWrite(pinReadingRFIDComplete, HIGH);
//    ReadValuesFromRFID(tagDataContainer); 
//         
//    ReportToRobotRFIDReadingComplete();
//  }
//  if(RobotRequestingWeightRead())
//  {
//    jarValidator.WeighJar();
//    
//    bool isWeightOK = jarValidator.CompareJarWeightWithOrderWeight(); 
//    if(isWeightOK)
//    {
//      ReportToRobotWeightReadingComplete();
//      ReportToRobotWeightOK();  
//        
//      String itemTrackerToPost = httpService->FormatItemTrackerPostData(tagDataContainer.orderID,tagDataContainer.itemID,COMPLETE);
//      httpService->Post(itemTrackerResource,itemTrackerToPost);   
//    }
//    else
//    {
//      ReportToRobotWeightReadingComplete();
//      
//      String itemTrackerToPost = httpService->FormatItemTrackerPostData(tagDataContainer.orderID,tagDataContainer.itemID,FAILED);
//      httpService->Post(itemTrackerResource,itemTrackerToPost);  
//    }
//  }
}

bool RobotRequestingRFIDRead()
{
  if(digitalRead(pinRequestedToReadRFID) == HIGH)
  {
    Serial.println("Robot requesting us to read RFID");
    isRFIDRead = true;
    digitalWrite(pinRequestedToReadRFID, LOW);
    return true;
  }
  else
  {
    return false;
  }
   
}

void ReportToRobotRFIDReadingComplete()
{
  digitalWrite(pinReadingRFIDComplete, HIGH);
  
}

bool RobotRequestingWeightRead()
{
  if(digitalRead(pinRequestedToReadWeight) == HIGH && isRFIDRead == true)
  {
    Serial.println("Robot requesting us to read weight");
    digitalWrite(pinRequestedToReadWeight, LOW);
    isRFIDRead = false;
    return true;
  }
  else
  {
    return false;
  }
 
}

void ReportToRobotWeightReadingComplete()
{
  digitalWrite(pinReadingWeightComplete, HIGH);
}

void ReportToRobotWeightOK()
{
   digitalWrite(pinReportToRobotWeightOK, HIGH);
}

void ReadValuesFromRFID(RFIDDataContainer& tagValues)
{
  Serial.println("Reading values from RFID tag...");
  
  tagValues.orderID = 7;
  tagValues.itemID = 20;
  tagValues.tearWeight = 20.00;
  tagValues.orderWeight = 25.00;
  jarValidator.SetTearWeight(tagDataContainer.tearWeight);
  jarValidator.SetOrderWeight(tagDataContainer.orderWeight);
  delay(200);
//    Serial.print("orderID: ");
//    Serial.println(tagDataContainer.orderID);
//    Serial.print("itemID: ");
//    Serial.println(tagDataContainer.itemID);
//    Serial.print("tearWeight: ");
//    Serial.println(tagDataContainer.tearWeight);
//    Serial.print("orderWeight: ");
//    Serial.println(tagDataContainer.orderWeight);
  Serial.println("RFID Reading complete. Letting robot know");
  
}
  



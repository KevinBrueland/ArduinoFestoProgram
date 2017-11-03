#include <ArduinoJson.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include "HttpService.h"
#include "CommonDefs.h"
#include "DataContainer.h"
#include "JarValidator.h"
#include "FestoWeight.h"
#include "FestoRFIDReader.h"

//constants
int baudRate = 9600;
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0xC0, 0x28};
byte apiServer[] = {52,178,208,12}; //api server address
String apiHost = "festorest.azurewebsites.net";
String itemTrackerResource = "/api/itemtrackers";
bool isRFIDRead = false;

//Pins - to be replaced with actual pin numbers
byte pinRequestedToReadRFID = 17;
byte pinReadingRFIDComplete = 14;
byte pinRequestedToReadWeight = 13;
byte pinReadingWeightComplete = 18;
byte pinReportToRobotWeightOK = 16;
byte pinReadWeight = A0;
byte pinReadFromRFID = 0;
byte pinWriteToRFID = 1;

//Instantiating our objects
IPAddress staticDeviceIp(192,168,1,136);
EthernetClient client;
DataContainer dataContainer;
HttpService httpService(&Serial, &client, apiServer, apiHost);
JarValidator jarValidator(&Serial, &dataContainer);
FestoWeight festoWeight(&Serial, pinReadWeight, &dataContainer);
FestoRFIDReader festoRFIDReader(&Serial1, &Serial, &dataContainer);


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
  ConfigurePins();  
  AttemptToEstablishEthernetConnection();    
}


void loop() 
{
  if(RobotRequestingRFIDRead())
  {
    festoRFIDReader.ReadRFID(); 
    ReportToRobotRFIDReadingComplete();
  }
  if(RobotRequestingWeightRead())
  {
    festoWeight.WeighJar();
    
    bool isWeightOK = jarValidator.CompareJarWeightWithOrderWeight(); 
    if(isWeightOK)
    {
      ReportToRobotWeightReadingComplete();
      ReportToRobotWeightOK();  
      UpdateItemStatusToComplete(); 
         
    }
    else
    {
      ReportToRobotWeightReadingComplete();
      UpdateItemStatusToFailed();
       
    }
  }
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

void UpdateItemStatusToComplete()
{
  String itemTrackerToPost = httpService.FormatItemTrackerPostData(dataContainer.orderID,dataContainer.itemID,COMPLETE);
  httpService.Post(itemTrackerResource,itemTrackerToPost);
}

void UpdateItemStatusToFailed()
{
  String itemTrackerToPost = httpService.FormatItemTrackerPostData(dataContainer.orderID,dataContainer.itemID,FAILED);
  httpService.Post(itemTrackerResource,itemTrackerToPost); 
}
  



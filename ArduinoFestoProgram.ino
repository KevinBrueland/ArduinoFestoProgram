#include <Ethernet.h>
#include <EthernetClient.h>
#include "HttpService.h"
#include "ItemStatusDefs.h"
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
//Pin allocation
byte pinRequestedToReadRFID = 17;
byte pinReadingRFIDComplete = 14;
byte pinRequestedToReadWeight = 15;
byte pinReadingWeightComplete = 22;
byte pinReportToRobotWeightOK = 16;
byte pinReadWeight = A0;


//Instantiating our objects
IPAddress staticDeviceIp(192,168,0,40);
IPAddress domainNameServer(192,168,12,1);
IPAddress gateway(192,168,0,2);
IPAddress subMask(255,255,255,0);
EthernetClient client;
DataContainer dataContainer;
FestoRFIDReader festoRFIDReader(&Serial1, &Serial, &dataContainer);
HttpService httpService(&Serial, &client, apiServer, apiHost);
JarValidator jarValidator(&Serial, &dataContainer);
FestoWeight festoWeight(&Serial, pinReadWeight, &dataContainer);

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

void StartSerials()
{
  Serial.begin(9600);
  Serial1.begin(38400);
}

void AttemptToEstablishEthernetConnection()
{
  Serial.println("Requesting IP from DHCP server...");
  int connection = Ethernet.begin(mac);
  Serial.print("connection: ");
  Serial.println(connection);
  if(connection == 1)
  {
    Serial.println("IP provided by DHCP"); 
    Serial.println("Device IP: ");
    Serial.println(Ethernet.localIP());
    Serial.println();
    Serial.println("Ethernet connection established");
  }
  else if(connection == 0)
  {
    Serial.println("Failed to connect to DHCP server");
    Serial.println("Using provided static ip address:");
    Serial.println(staticDeviceIp);
    Ethernet.begin(mac, staticDeviceIp, domainNameServer, gateway, subMask);
    Serial.println("Ethernet connection established");
  }
    
}

void setup() 
{
  StartSerials();
  ConfigurePins();
  //AttemptToEstablishEthernetConnection();
  
  
}
void loop() 
{ 
  
  if(RobotAsksUsToReadRFID())
  {
    festoRFIDReader.ReadRFID(); 
    ReportToRobotRFIDReadingComplete();
  }
  
  if(RobotAsksUsToReadWeight())
  {
    festoWeight.WeighJar();
    
    bool isWeightOK = true; //forcing order to be valid //jarValidator.CompareJarWeightWithOrderWeight(); 
    if(isWeightOK)
    {
      ReportToRobotWeightReadingComplete();
      ReportToRobotWeightOK();  
      //UpdateItemStatusToComplete(); 
      Serial.print("Updating item to COMPLETE in DB");   
    }
    else
    {
      ReportToRobotWeightReadingComplete();
      //UpdateItemStatusToFailed();
       Serial.print("Updating item to FAILED in DB"); 
       
    }
  }

}

bool RobotAsksUsToReadRFID()
{
  if(digitalRead(pinRequestedToReadRFID) == LOW)
  {
    Serial.println("Robot requesting us to read RFID");
    isRFIDRead = true;
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

bool RobotAsksUsToReadWeight()
{
  if(digitalRead(pinRequestedToReadWeight) == LOW && isRFIDRead == true)
  {
    Serial.println("Robot requesting us to read weight");
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
  String itemTrackerToPost = httpService.FormatItemTrackerPostData(dataContainer.orderID,dataContainer.itemID,dataContainer.measuredWeight, COMPLETE);
  httpService.Post(itemTrackerResource,itemTrackerToPost);
}

void UpdateItemStatusToFailed()
{
  String itemTrackerToPost = httpService.FormatItemTrackerPostData(dataContainer.orderID,dataContainer.itemID, dataContainer.measuredWeight, FAILED);
  httpService.Post(itemTrackerResource,itemTrackerToPost); 
}
  



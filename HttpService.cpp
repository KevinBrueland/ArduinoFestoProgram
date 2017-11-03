#include "Arduino.h"
#include "HttpService.h"
#include "CommonDefs.h"

HttpService::HttpService(Stream *serial, EthernetClient *client, byte* apiServer, String apiHost)
{
  _apiServer = apiServer;
  _apiHost = apiHost;
  _client = client;
  _serial = serial;
}
void HttpService::Get(String resource)
{
  if(AttemptConnectionToServer() == true)
  {
    _serial->println("Connection to " + _apiHost + " establised");
    _serial->println("GET " + resource + " HTTP/1.1");
    _serial->println("Host: " + _apiHost);
    _client->println("GET " + resource + " HTTP/1.1");    
    _client->println("Host: " + _apiHost);       
    _client->println("Connection: close"); 
    _client->println("Content-Type: application/json; charset=UTF-8");
    _client->println(); 
  }
  else
  {
    _serial->println("Connection to " + _apiHost + " failed");
  }

}

void HttpService::Post(String resource, String dataToPost)
{
  if(AttemptConnectionToServer() == true)
  {
    _serial->println("Connection to " + _apiHost + "establised");
    _client->println("POST " + resource + " HTTP/1.1" );
    _client->println("Host: " + _apiHost);    
    _client->println("Connection: close"); 
    _client->println("Content-Type: application/json; charset=UTF-8");
    _client->print("Content-Length: "); 
    _client->println(dataToPost.length());
    _client->println();
    _client->println(dataToPost);  
  }
  else
  {
    _serial->println("Connection to " + _apiHost + " failed");
  }
}

bool HttpService::AttemptConnectionToServer()
{
  _serial->println("Attempting to connect to " + (String)_apiHost + "...");
  if(_client->connect(_apiServer,80))
  {
    return true;
  }
  else
  {
    return false;
  }
}

String HttpService::FormatItemTrackerPostData(int currentOrderID, int currentItemID, float measuredWeight, int itemStatus)
{
  char postData[250];

  if(itemStatus == COMPLETE)
  {
    char startString[] = "{\r\n  \"itemStatus\": 2,\r\n  \"itemID\":";
    char sep1String[] = ",\r\n  \"orderID\":";
    char sep2String[] = ",\r\n  \"measuredWeight\":";
    char endString[] = "\r\n}";

    sprintf(postData,"%s %i%s %i%f %s",startString, currentItemID, sep1String, currentOrderID, sep2String, measuredWeight, endString);
    

    String FormattedPostData(postData);
    return FormattedPostData;
  }
  else if(itemStatus == FAILED)
  {
    char startString[] = "{\r\n  \"itemStatus\": 3,\r\n  \"itemID\":";
    char sep1String[] = ",\r\n  \"orderID\":";
    char sep2String[] = ",\r\n  \"measuredWeight\":";
    char endString[] = "\r\n}";

    sprintf(postData,"%s %i%s %i%f %s",startString, currentItemID, sep1String ,currentOrderID, sep2String, measuredWeight, endString);

    String FormattedPostData(postData);
    return FormattedPostData;
  }

  
}

void HttpService::ShowDataInMonitor()
{
  if(_client->available())
  {
    char c = _client->read();
    _serial->print(c);
  }
}




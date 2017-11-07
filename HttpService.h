#ifndef HttpService_h;
#define HttpService_h;
#include "Arduino.h";
#include "EthernetClient.h"
#include "Ethernet.h"


class HttpService
{
  
  public:
    HttpService(Stream *serial, EthernetClient *client, byte* apiServer, String apiHost);
    void Get(String resource);
    void Post(String resource, String dataToPost); 
    String FormatItemTrackerPostData(int orderId, int itemId, float measuredWeight, int itemStatus);
    void ShowDataInMonitor();    
    
    
    
  private:
  bool AttemptConnectionToServer();
  
  String _apiHost;
  byte* _apiServer;
  EthernetClient *_client;
  Stream *_serial;
  
  
};

#endif

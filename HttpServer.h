#include <ESP8266WebServer.h>

class HttpServer{
 private:
  ESP8266WebServer configurationHttp{80}; //creating the server at port 80
  struct memory_s *eeprom;
  #include "page.html.h"

  String toString(uint32_t ip){
    return String(ip >> 0 & 0xFF) + "." + 
      String(ip >> 8 & 0xFF) + "." + 
      String(ip >> 16 & 0xFF) + "." + 
      String(ip >> 24 & 0xFF);
  }

  void createPage(){
    String copy = ris;
    copy.replace("SSS", eeprom->ssid);
    copy.replace("PPP", eeprom->password);
    copy.replace("CCC", eeprom->is_client?"checked":"");
    copy.replace("TTT", String(eeprom->timeout_connect_seconds));
    copy.replace("DDD", eeprom->is_dhcp?"checked":"");
    copy.replace("XXX", toString(eeprom->ip));
    copy.replace("YYY", toString(eeprom->netmask));
    copy.replace("ZZZ", toString(eeprom->gateway));
    copy.replace("BBB", String(eeprom->baudrate));
    configurationHttp.send(200, "text/html", copy);
  }

  void readPage(){
    configurationHttp.arg("ssid").toCharArray(eeprom->ssid, 33);
    configurationHttp.arg("password").toCharArray(eeprom->password, 64);
    eeprom->is_client = configurationHttp.arg("client") != "";
    eeprom->is_dhcp = configurationHttp.arg("dhcp") != "";

    Serial.println(configurationHttp.arg("ssid"));
    Serial.println(configurationHttp.arg("password"));
    Serial.println(eeprom->ssid);
    Serial.println(eeprom->password);
    Serial.println(eeprom->is_client ?"isClient":"isAP");
    Serial.println(eeprom->is_dhcp ?"isDHCP":"isStatic");
     
    Serial.println(configurationHttp.arg("timeout"));
    Serial.println(configurationHttp.arg("ip"));
    Serial.println(configurationHttp.arg("subnet"));
    Serial.println(configurationHttp.arg("gateway"));
    Serial.println(configurationHttp.arg("baudrate"));
    writePreferences();

    createPage();
  }
 public:
  HttpServer(struct memory_s *eeprom){
    this->eeprom = eeprom;
  }

  void begin(){
    configurationHttp.on("/", HTTP_GET, std::bind(&HttpServer::createPage, this));
    configurationHttp.on("/", HTTP_POST, std::bind(&HttpServer::readPage, this));
    configurationHttp.begin();
  }
  
  void update(){
    configurationHttp.handleClient(); 
  }
};

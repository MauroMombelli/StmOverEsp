#include "Configuration.h"

#include <WiFi.h>

class Programmer{
 private:
  WiFiServer configuration{1235};
  
  const int pinBoot = 5;
  const int pinReset = 4;
  struct memory_s *eeprom;
  
 public:
  
  Programmer(struct memory_s *eeprom){
    pinMode(pinReset, OUTPUT);
    pinMode(pinBoot, OUTPUT);
    digitalWrite(pinReset, HIGH);
    digitalWrite(pinBoot, HIGH);
    
    this->eeprom = eeprom;
  }

  void begin(){
    configuration.begin();
  }
  
  void startUploadStm(){
    Serial.flush();
    Serial.begin(115200, SERIAL_8E1);
    digitalWrite(pinBoot, LOW);
    delay(50);
    digitalWrite(pinReset, LOW);
    delay(50);
    digitalWrite(pinReset, HIGH);
    delay(50);
    digitalWrite(pinBoot, HIGH);
  }
  
  void endUploadStm(){
    Serial.begin(eeprom->baudrate, SERIAL_8N1);
  }
  
  void resetStm(){
    digitalWrite(pinReset, LOW);
    delay(100);
    digitalWrite(pinReset, HIGH);
  }
  
  void update(){
    static WiFiClient client = configuration.available();
    if (!client){
      client = configuration.available();
      if (client){
        client.write("Welcome to Lesto's EspStmRemote\r\n");
      }
    }
    if (client){
      if (client.connected()){
        switch(client.read()){
          case 'r':
            client.write("Reset requested\r\n");
            resetStm();
            break;
          case 'u':
            client.write("Start upload requested\r\n");
            startUploadStm();
            break;
          case 'e':
            client.write("End upload requested\r\n");
            endUploadStm();
            break;
        }
      }
    }
  }
};

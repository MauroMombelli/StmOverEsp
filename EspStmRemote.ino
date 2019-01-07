
#define DEBUG true

const short int BUILTIN_LED1 = 2;

#include <ESP8266WiFi.h>
#include "Connection.h"
#include "Configuration.h"
#include "Programmer.h"
#include "SerialBridge.h"
#include "HttpServer.h"

Programmer prog(&eeprom.preferences);
SerialBridge bridge(&eeprom.preferences);
HttpServer gui(&eeprom.preferences);

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Serial.println("start");
  EEPROM.begin(512);
  resetPreferences();
  readPreferences();
  connect(eeprom.preferences);
  prog.begin();
  bridge.begin();
  gui.begin();
}

void loop() {
  prog.update();
  bridge.update();
  gui.update();
}

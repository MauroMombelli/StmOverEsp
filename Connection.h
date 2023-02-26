#pragma once
#include "Configuration.h"

bool tryConnectAsClient(struct memory_s eeprom){
  char ssid[33], password[65];

  strncpy(ssid, eeprom.ssid, 32);
  ssid[32] = '\0'; //be sure there is an end of string

  strncpy(password, eeprom.password, 64);
  password[64] = '\0'; //be sure there is an end of string
  
  if (DEBUG) Serial.printf("CONNECTING AS CLIENT TO %s %s\n", ssid, password);
  WiFi.begin(ssid, password);
  //eventully set static IP
  if (!eeprom.is_dhcp){
    if (DEBUG) Serial.println("CONFIG STATIC IP");
    WiFi.config( IPAddress(eeprom.ip), IPAddress(eeprom.netmask), IPAddress(eeprom.gateway) );
  }

  if (DEBUG) Serial.println("CONNECTING");
  
  bool status = true;
  uint64_t endTimeout = millis() + eeprom.timeout_connect_seconds * 1000UL;
  while (WiFi.status() != WL_CONNECTED && (eeprom.timeout_connect_seconds == 0 || millis() < endTimeout) ){
    if (DEBUG) Serial.println("CONNECTING...");
    delay(500);
    digitalWrite(BUILTIN_LED1, !status);
    status = !status;
  }
  
  if (DEBUG) (WiFi.status() == WL_CONNECTED)? Serial.println("CONNECTED"):Serial.println("NOT CONNECTED");
  if (DEBUG) Serial.println(WiFi.localIP()); 

  return WiFi.status() == WL_CONNECTED;
}

bool tryConnectAsAP(struct memory_s eeprom){
  char ssid[33], password[65];

  strncpy(ssid, eeprom.ssid, 32);
  ssid[32] = '\0'; //be sure there is an end of string

  strncpy(password, eeprom.password, 64);
  password[64] = '\0'; //be sure there is an end of string
  
  if (DEBUG) Serial.printf("CONNECTING AS AP %s %s\n", ssid, password);
  WiFi.softAPConfig(IPAddress(eeprom.ip), IPAddress(eeprom.gateway), IPAddress(eeprom.netmask));
  WiFi.softAP(ssid, password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  return true;
}

void connect(struct memory_s eeprom){
  bool connected =  false;
  
  if (eeprom.is_client){
    connected = tryConnectAsClient(eeprom);
    if (!connected){
      resetPreferences(&eeprom);
    }
  }
  
  if (!connected){
    tryConnectAsAP(eeprom);
  }
}

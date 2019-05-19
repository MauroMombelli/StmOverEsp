#pragma once

#include <EEPROM.h>

#pragma pack(1)
struct memory_s{
  char ssid[33];
  char password[64];
  uint32_t ip;
  uint32_t netmask;
  uint32_t gateway;
  uint32_t baudrate;
  uint16_t version;
  uint8_t timeout_connect_seconds;
  struct{
    uint8_t is_client:1;
    uint8_t is_dhcp:1;
  };
};
#pragma pack(0)

union eeprom_u{
  uint8_t raw_byte[sizeof(struct memory_s)];
  struct memory_s preferences;
};

bool validatePreferences(struct memory_s *p){
  bool ok = true;
  
  if ( (p->ssid[0] < 'a' || p->ssid[0] > 'z') && (p->ssid[0] <= 'A' || p->ssid[0] >= 'Z') ) 
    return false;
  
  switch (p->version){
    case 1:
      //ok
      break;
    default:
      ok = false;
  }

  return ok;
}

void resetPreferences(struct memory_s *eeprom){
  //data on eeprom is invalid. set default values ans save it
  eeprom->ssid[0] = 'L';
  eeprom->ssid[1] = 'E';
  eeprom->ssid[2] = 'S';
  eeprom->ssid[3] = 'T';
  eeprom->ssid[4] = 'O';
  eeprom->ssid[5] = '\0';
  
  eeprom->password[0] = 'l';
  eeprom->password[1] = 'e';
  eeprom->password[2] = 's';
  eeprom->password[3] = 't';
  eeprom->password[4] = 'o';
  eeprom->password[5] = 'f';
  eeprom->password[6] = 'a';
  eeprom->password[7] = 'n';
  eeprom->password[8] = 't';
  eeprom->password[9] = 'e';
  eeprom->password[10] = '\0';

  eeprom->ip = 192 << 0 | 168 << 8 | 0 << 16 | 123 << 24;

  eeprom->netmask = 255 << 0 | 255 << 8 | 255 << 16 | 0 << 24;

  eeprom->gateway = eeprom->ip;

  eeprom->baudrate = 115200;

  eeprom->version = 1;

  eeprom->timeout_connect_seconds = 10;

  eeprom->is_client = 0;
  eeprom->is_dhcp = 0;
}

void writePreferences(union eeprom_u *rawEeprom){
  
  for (uint8_t i = 0; i < sizeof(struct memory_s); i++){
    EEPROM.write(i, rawEeprom->raw_byte[i]);
  }
  EEPROM.commit();

  ESP.restart();
}

void readPreferences(union eeprom_u *rawEeprom){
  for (uint8_t i = 0; i < sizeof(struct memory_s); i++){
    rawEeprom->raw_byte[i] = EEPROM.read(i);
  }

  rawEeprom->preferences.ssid[32] = '\0';
  rawEeprom->preferences.password[63] = '\0';
  if (DEBUG) Serial.printf("Readed SSID %c%c%c%c\n\r", rawEeprom->preferences.ssid[0], rawEeprom->preferences.ssid[1], rawEeprom->preferences.ssid[2], rawEeprom->preferences.ssid[3]);
  if ( !validatePreferences( &(rawEeprom->preferences) ) ){
    if (DEBUG) Serial.printf("Invalid preferences, resetting\r\n");
    resetPreferences(&rawEeprom->preferences);
    writePreferences(rawEeprom);
  }
}

union eeprom_u eeprom;

void writePreferences(){
  writePreferences(&eeprom);
}

void resetPreferences(){
  resetPreferences(&(eeprom.preferences));
}

void readPreferences(){
  readPreferences(&eeprom);
}

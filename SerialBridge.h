class SerialBridge{
 private:
  WiFiServer server{1234};
  struct memory_s *eeprom;
 public:  
 
  SerialBridge(struct memory_s *eeprom){
    this->eeprom = eeprom;
  }
  
  void begin(){
    if (DEBUG) {Serial.println("setting RX buffer"); Serial.flush();}
    Serial.setRxBufferSize(1024);
    if (DEBUG) {Serial.printf("setting baudrate to %d\r\n", eeprom->baudrate); Serial.flush();}
    Serial.begin(eeprom->baudrate, SERIAL_8N1);
    server.begin();
  }
  
  void update(){
    static WiFiClient client = server.available();
    if (!client){
      client = server.available();
      
      digitalWrite(BUILTIN_LED1, HIGH);
      //flush all the buffer
      while(Serial.available()){
        Serial.read();
      }
    }
    if (client){
      if (client.connected()){
        uint8_t timeout = 100;
        while ( client.available() && --timeout )
        {
          Serial.write( client.read() );
        }
        timeout = 100;
        while ( Serial.available() && --timeout )
        {
          client.write( Serial.read() );
        }
      }
      digitalWrite(BUILTIN_LED1, LOW);
    }
  }
};

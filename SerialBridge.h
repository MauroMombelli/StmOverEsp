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
    }
    if (client){
      digitalWrite(BUILTIN_LED1, HIGH);
  
      //flush all the buffer
      while(Serial.available()){
        Serial.read();
      }
      
      while (client.connected()){
  
        while ( client.available() )
        {
          Serial.write( client.read() );
        }
      
        while ( Serial.available() )
        {
          client.write( Serial.read() );
        }
      }
      digitalWrite(BUILTIN_LED1, LOW);
    }
  }
};

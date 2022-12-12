void readSerial(){
  if (Serial.available()) {
    String str = "";
    Serial.print("Input: ");
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c != 10 && c != 13) { // Don't send line end characters to HM10.
          str += c;
        }
      }
    }

    if (strstr(str.c_str(), BLE_TAG)){
      Serial.print("BLE Stream: ");
      str = strremove(str.c_str(), BLE_TAG);
      BLESerial.print(str);
    }

    if (strstr(str.c_str(), NBIOT_TAG)){
      Serial.print("NB-IoT Stream: ");
      str = strremove(str.c_str(), NBIOT_TAG);
      str = strcat(str.c_str(), "\r\n");
      NBIOTSerial.write(str.c_str());
    }
    Serial.println(str);
  }
}

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

void checkReset(){
  if(strstr(outputNBIOT.c_str(), RESET_NBIOT_TAG)) {
    Serial.println("NBIoT Reset done.");
    resetState = false;
    setupNBIOTState = true;
  }
}

void checkOk(){
  if(!resetState){
    if(strstr(outputNBIOT.c_str(), "OK")){
      Serial.println("Operation done, go with the next one");
      okNBIOTList++;
    }
  }
}

void checkConnection(){
  if(!resetState){
    if(strstr(outputNBIOT.c_str(), "CEREG: 5")){
      Serial.println("Connected!");
      delay(200);
      NBIOTSerial.write("AT+CGATT?\r\n");

    }
  }
}

void checkRSSI(){
  if(!resetState){
    
  }
}

void readNBIOT(){
  if(NBIOTSerial.available()){ 
    outputNBIOT=NBIOTSerial.readStringUntil('\n');
    Serial.println(outputNBIOT);
    checkReset();
    checkConnection();
    checkOk();
    checkRSSI();
  }
}

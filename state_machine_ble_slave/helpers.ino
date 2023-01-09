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

void readNBIOT(){
  if(NBIOTSerial.available()){ 
    outputNBIOT=NBIOTSerial.readStringUntil('\n');
    Serial.println(outputNBIOT);
  }
}


void checkOkBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      Serial.println("Operation ble done, go with the next one");
      bleOperationIndex++;
    }
    if(strstr(outputBLE.c_str(), "AT+NAME")){
      Serial.println("Operation name ble done, go with the next one");
      bleOperationIndex++;
    }
  }
}

void checkResetBLE(){
  if(strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println("BLE setup done.");
    BLESerial.write("AT+NAME?");

    setupBLEState = false;
    bleAdvState = true;
  }
}

void readBLE(){
  if (BLESerial.available()) {
    // String outputBLE = "";
    Serial.print("HM10: ");
    
    outputBLE = BLESerial.readStringUntil('\n');
    // prevMillis = millis();
    // while (millis() - prevMillis < READ_TIME) {
    //   if (BLESerial.available()) {
    //     outputBLE += (char) BLESerial.read();
    //   }
    // }
    Serial.println(outputBLE);
    checkOkBLE();
    checkResetBLE();
  }  
}
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

void checkOkBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      Serial.println("Operation ble done, go with the next one");
      bleOperationIndex++;
    }
  }
}

void checkDisconnBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+LOST")){
      Serial.println("BLE DISCONNECTED, go with the next one");
      digitalWrite(LED_BUILTIN, LOW);
      bleOperationIndex++;
    }
  }
}

void checkResetBLE(){
  if(strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println("BLE setup done.");
    BLESerial.write("AT+NAME?");
    digitalWrite(LED_BUILTIN, LOW);


  if(strstr(stateMachine.ActiveStateName(), "SETUP_BLE")){
    bleOperationIndex = 0;
    oldStateBle = -1;
    setupBLEState = false;
    setupNBIOTState = true;
    }
  }
}

void checkResetNBIOT() {
  if (strstr(outputNBIOT.c_str(), RESET_NBIOT_TAG)) {
    Serial.println("NBIoT Reset done.");
    resetState = false;
    setupBLEState = true;
  }
}

void checkConnectionNBIOT() {
    if (strstr(outputNBIOT.c_str(), CEREG_NBIOT_TAG)) {
      Serial.println("Connected!");
      delay(200);
      NBIOTSerial.write(CGATTcmd.c_str());
      // TODO: check if CGATT returns 1
    }

}

void checkOkNBIOT(){
    if (strstr(outputNBIOT.c_str(), SUCCESS_NBIOT_TAG)) {
      // Serial.println("Operation done, go with the next one");
      okNBIOTList++;
    }
}


void readBLE(){
  if (BLESerial.available()) {
    Serial.print("HM10: ");
    
    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    checkOkBLE();
    checkResetBLE();
  }  
}

void readNBIOT() {
  if (NBIOTSerial.available()) {
    outputNBIOT = NBIOTSerial.readStringUntil('\n');
    Serial.println(outputNBIOT);
    
    if(strstr(stateMachine.ActiveStateName(), "RESET")){
      checkResetNBIOT();
    }

    if(strstr(stateMachine.ActiveStateName(), "SETUP_NBIOT")){
      checkOkNBIOT();
      checkConnectionNBIOT();
    }
  }
}
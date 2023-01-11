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


  if(strstr(stateMachine.ActiveStateName(), "S_BLE")){
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
      NBIOTSerial.write(CGATTcmd);
      // TODO: check if CGATT returns 1
    }

}

void checkOkNBIOT(){
    if (strstr(outputNBIOT.c_str(), SUCCESS_NBIOT_TAG)) {
      // Serial.println("Operation done, go with the next one");
      okNBIOTList++;
    }
}

void checkSendNBIOT(){
  if (strstr(outputNBIOT.c_str(), "0,7")){
    Serial.println("Datagram sent");
    sleepState = true;
  }
}

void checkRSSI() {
    if (strstr(outputNBIOT.c_str(), "RSRQ")) {
      strcpy(rsrq, strremove(strremove(outputNBIOT.c_str(), "\"RSRQ\",-"), "\r"));
      // convert rsrq in int 
      rsrqInt = atoi(rsrq);
      Serial.println(rsrqInt);
      if (rsrqInt < RSRQ_THRESHOLD && rsrqInt > 0){      
        // if > 190 master
        memset(payload, 0, sizeof payload);
        memset(TRANScmd, 0, sizeof TRANScmd);

        createMessage();
        Serial.println("Sending ...");
        readyToSendNBIOT = true;
      } else {
        Serial.println("ERROR: RSRQ too low for the transmission");  
        Serial.println("Setting up master mode");   
        masterState = true; 
      }
  }
}

void createMessage() {
  if (idDatagram < 10) {
    stringIdDatagram = "00" + String(idDatagram);
  }
  if (idDatagram >= 10 && idDatagram < 100) {
    stringIdDatagram = "0" + String(idDatagram);
  }
  if (idDatagram >= 100 && idDatagram < 1000) {
    stringIdDatagram = String(idDatagram);
  }

  strcat(payload, stringIdDatagram.c_str());
  strcat(payload, "A");
  strcat(payload, rsrq);

  Serial.println(payload);

  payloadLen = strlen(payload);
  for (int i = 0, j = 0; i < payloadLen; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }

  Serial.println(payloadHex);

  strcat(TRANScmd, "AT+NSOST=0,\"131.175.120.22\",8883,");
  snprintf(buffer, sizeof(buffer), "%d", payloadLen);
  strcat(TRANScmd, buffer);
  strcat(TRANScmd, ",\"");
  strcat(TRANScmd, payloadHex);
  strcat(TRANScmd, "\"\r\n");
  Serial.println(TRANScmd);
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
    
    if(strstr(stateMachine.ActiveStateName(), "RST")){
      checkResetNBIOT();
    }

    if(strstr(stateMachine.ActiveStateName(), "S_NB")){
      checkOkNBIOT();
      checkConnectionNBIOT();
    }

    if(strstr(stateMachine.ActiveStateName(), "WAKE")){
      checkRSSI();
      checkSendNBIOT();
    }
  }
}
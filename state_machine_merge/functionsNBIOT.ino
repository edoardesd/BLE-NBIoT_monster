

void checkResetNBIOT() {
  if (strstr(outputNBIOT.c_str(), RESET_NBIOT_TAG)) {
    Serial.println(F("NBIoT Reset done"));
    resetState = false;
    setupBLEState = true;
  }
}

void checkConnectionNBIOT() {
    if (strstr(outputNBIOT.c_str(), CEREG_NBIOT_TAG)) {
      Serial.println(F("Conn!"));
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
  if (strstr(outputNBIOT.c_str(), "0,10")){
    Serial.println(F("Sent"));

    if(strlen(forwardPayload) > 1){
      forwardState = true;
    } else {
      sleepState = true;
    }
  }
}

void prepareConnBLE(){
  Serial.println(F("ERROR: RSRQ low... Set up master"));  
  forceBLE = false; 
  masterState = true; 
}


void checkRSSI() {
  if (strstr(outputNBIOT.c_str(), "RSRQ")) {
    strcpy(rsrq, strremove(strremove(outputNBIOT.c_str(), "\"RSRQ\",-"), "\r"));
    rsrqInt = atoi(rsrq);  // convert rsrq in int 
    Serial.println(rsrqInt);

    createPayload();
    createHexPayload();
    createMessage();

    if(forceBLE){
      prepareConnBLE();
    } 
    if(!forceBLE){
      if (rsrqInt < RSRQ_THRESHOLD && rsrqInt > 0){  
        Serial.println(F("Sending"));
        readyToSendNBIOT = true;
      } else {
        prepareConnBLE();
      }
    }
}
}



void createPayload(){
  memset(payload, 0, sizeof payload);
  createIdDatagram();

  strcat(payload, BLENAME);
  strcat(payload, "A");
  strcat(payload, stringIdDatagram.c_str());
  strcat(payload, "A");
  strcat(payload, rsrq);

  // Serial.println(payload);
}

void createHexPayload(){
  memset(payloadHex, 0, sizeof payloadHex);
  payloadLen = strlen(payload);
  for (i = 0, j = 0; i < payloadLen; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }

  // Serial.println(payloadHex); // Divide payload
}

void createMessage() {
  memset(TRANScmd, 0, sizeof TRANScmd);
  memset(buffer, 0, sizeof buffer);
  strcat(TRANScmd, "AT+NSOST=0,\"131.175.120.22\",8883,");
  snprintf(buffer, sizeof(buffer), "%d", strlen(payloadHex)/2); 
  strcat(TRANScmd, buffer);
  strcat(TRANScmd, ",\"");
  strcat(TRANScmd, payloadHex);
  strcat(TRANScmd, "\"\r\n");
  Serial.println(TRANScmd);
  memset(payloadHex, 0, sizeof payloadHex);
}
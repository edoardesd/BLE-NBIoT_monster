
void checkResetNBIOT() {
  if (strstr(outputNBIOT.c_str(), RESET_NBIOT_TAG)) {
    Serial.println(F("NB Rst"));
    resetState = false;
    setupBLEState = true;
  }
}

void checkConnectionNBIOT() {
    if (strstr(outputNBIOT.c_str(), CEREG_NBIOT_TAG)) {
      Serial.println(F("Conn"));
      delay(200);
      NBIOTSerial.write(CGATTcmd);
      // TODO: check if CGATT returns 1
    }

}

void checkOkNBIOT(){
    if (strstr(outputNBIOT.c_str(), SUCCESS_NBIOT_TAG)) {
      okNBIOTList++;
    }
}

void onSendNBIOT(){
  wakeState = false;
  totalTransmissions++;
  // Serial.println(totalTransmissions);
  if(strlen(forwardPayload) > 1){
    forwardState = true;
  } else {
    sleepState = true;
  }
}

void checkSendNBIOT(){
  if (strstr(outputNBIOT.c_str(), "0,10")){
    Serial.println(F("Snt"));
    onSendNBIOT();
  }
}

void prepareConnBLE(){
  Serial.println(F("ERR RSRQ low"));  
  masterState = true; 
}


void setupDatagram() {
  strlcpy(current_rsrq, "167", sizeof(current_rsrq));
  #ifdef NONBMODULE  
    Serial.println(F("RSSI skp")); 
    strcpy(current_rsrq, "129");
  #endif
  // uint8_t rsrqInt = atoi(current_rsrq);  // convert rsrq in int 
  createPayload(current_rsrq);
  createHexPayload();
  createMessage();

  if(!forceNBIOT){
    Serial.println(F("Ready to snd"));
    readyToSendNBIOT = true;
  } else {
    prepareConnBLE();
  }
}

void createPayload(char *rsrq){
  memset(payload, 0, sizeof payload);
  char idDatagram[4];
  createIdDatagram(idDatagram);
  // String idDatagram = createIdDatagram();
  // Serial.println(idDatagram);
  strlcat(payload, BLENAME, sizeof(payload));
  strlcat(payload, "A", sizeof(payload));
  strlcat(payload, idDatagram, sizeof(payload));
  strlcat(payload, "A", sizeof(payload));
  strlcat(payload, rsrq, sizeof(payload));
}

void createHexPayload(){
  memset(payloadHex, 0, sizeof payloadHex);
  uint8_t payloadLen = strlen(payload);
  for (i = 0, j = 0; i < payloadLen; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }
  // free(payloadLen);
}

void createMessage() {
  memset(TRANScmd, 0, sizeof TRANScmd);
  char buffer[5];
  strlcat(TRANScmd, FIXED_DATAGRAM, sizeof(TRANScmd));
  snprintf(buffer, sizeof(buffer), "%d", strlen(payloadHex)/2); 
  strlcat(TRANScmd, buffer, sizeof(TRANScmd));
  strlcat(TRANScmd, ",\"", sizeof(TRANScmd));
  strlcat(TRANScmd, payloadHex, sizeof(TRANScmd));
  strlcat(TRANScmd, "\"\r\n", sizeof(TRANScmd));
  Serial.println(TRANScmd);
}

void checkErrorNBIOT(){
  if (strstr(outputNBIOT.c_str(), "ERROR: 50")){
    Serial.println(F("WRN: Err send"));
    readyToSendNBIOT = false;
    wakeState = false;
    isLostConn = false;
    forwardState = false;
    sleepState = true;
  }
}
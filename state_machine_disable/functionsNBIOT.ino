
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
  strcpy(current_rsrq, "167");
  current_rsrq[4] = '\0';
  #ifdef NONBMODULE  
    Serial.println(F("RSSI skp")); 
    strcpy(current_rsrq, "129");
  #endif
  uint8_t rsrqInt = atoi(current_rsrq);  // convert rsrq in int 
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
  String idDatagram = createIdDatagram();
  strcat(payload, BLENAME);
  strcat(payload, "A");
  strcat(payload, idDatagram.c_str());
  strcat(payload, "A");
  strcat(payload, rsrq);
}

void createHexPayload(){
  memset(payloadHex, 0, sizeof payloadHex);
  uint8_t payloadLen = strlen(payload);
  for (i = 0, j = 0; i < payloadLen; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }
}

void createMessage() {
  memset(TRANScmd, 0, sizeof TRANScmd);
  char buffer[5] = "";
  strcat(TRANScmd, FIXED_DATAGRAM);
  snprintf(buffer, sizeof(buffer), "%d", strlen(payloadHex)/2); 
  strcat(TRANScmd, buffer);
  strcat(TRANScmd, ",\"");
  strcat(TRANScmd, payloadHex);
  strcat(TRANScmd, "\"\r\n");
  Serial.println(TRANScmd);
}
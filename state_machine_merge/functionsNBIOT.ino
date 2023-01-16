

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

void checkRSSI() {
  if (strstr(outputNBIOT.c_str(), "RSRQ")) {
    strcpy(rsrq, strremove(strremove(outputNBIOT.c_str(), "\"RSRQ\",-"), "\r"));
    rsrqInt = atoi(rsrq);  // convert rsrq in int 
    Serial.println(rsrqInt);
    
    // memset(payload, 0, sizeof payload);
    // memset(TRANScmd, 0, sizeof TRANScmd);

    createPayload();
    createHexPayload();
    createMessage(payloadHex);

    if (rsrqInt < RSRQ_THRESHOLD && rsrqInt > 0){      
      Serial.println(F("Sending"));
      readyToSendNBIOT = true;
    } else {
      Serial.println(F("ERROR: RSRQ low"));  
      Serial.println(F("Set up master"));   
      masterState = true; 
    }
  }
}


void createPayload(){
  payload[0] = '\0';
  createIdDatagram();

  strcat(payload, BLENAME);
  strcat(payload, "A");
  strcat(payload, stringIdDatagram.c_str());
  strcat(payload, "A");
  strcat(payload, rsrq);

  Serial.println(payload);
}

void createHexPayload(){
  payloadHex[0] = '\0';
  payloadLen = strlen(payload);
  for (i = 0, j = 0; i < payloadLen; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }

  Serial.println(payloadHex); // Divide payload
}

void createMessage(char *currentHex) {
  TRANScmd[0] = '\0';
  buffer[0] = '\0';
  strcat(TRANScmd, "AT+NSOST=0,\"131.175.120.22\",8883,");
  snprintf(buffer, sizeof(buffer), "%d", strlen(currentHex)/2); 
  strcat(TRANScmd, buffer);
  strcat(TRANScmd, ",\"");
  strcat(TRANScmd, currentHex);
  strcat(TRANScmd, "\"\r\n");
  Serial.println(TRANScmd);
}
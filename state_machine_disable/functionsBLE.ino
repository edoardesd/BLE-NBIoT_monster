void checkOkBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      bleOperationIndex++;
    }
  }
}

void checkDisconnBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+LOST")){
      Serial.println(F("BLE DISCON"));
      digitalWrite(LED_BUILTIN, LOW);
      isLostConn = true;
    }
  }
}

void checkResetBLE(){
  if(strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println(F("BLE set"));
    BLESerial.write("AT+NAME?");
    digitalWrite(LED_BUILTIN, LOW);


  if(strstr(stateMachine.ActiveStateName(), "BLE_S")){
    bleOperationIndex = 0;
    oldStateBle = -1;
    setupBLEState = false;
    setupNBIOTState = true;
  }

  // discconnected
     if(strstr(stateMachine.ActiveStateName(), "BLE_DISC")){
      bleOperationIndex = 0;
      oldStateBle = -1;
      isLostConn = false;
      disconnectedState = false;
      sleepState = true;
      Serial.println(F("DisconnDone"));

    }
  }
}

void checkMessageBLE(){
  if(strstr(outputBLE.c_str(), "6d")){
    Serial.println(F("Rqst from node"));
    strcpy(forwardPayload, outputBLE.c_str());
  }
}

void checkDiscovery(){
  if(strstr(outputBLE.c_str(), "OK+DISC")){
    char mac[13];
    char *mac_start = strchr(outputBLE.c_str(), ':');
    uint8_t index = (uint8_t)(mac_start - outputBLE.c_str()+1);
    strncpy(mac, &outputBLE.c_str()[index], 12);
    mac[12]='\0';

    if(strstr(mac, MAC_TO_CONNECT)){
      Serial.println(F("MAC fnd"));
      mac_found = true;
    }
  }

  if(strstr(outputBLE.c_str(), "OK+DISCE")){
    if(mac_found){
      char connectCMD[13];
      strcat(connectCMD, "AT+CON");
      strcat(connectCMD, MAC_TO_CONNECT);
      Serial.println(connectCMD);
      BLESerial.write(connectCMD);
    }else{
      Serial.println(F("WARN no Mac "));
      masterState = false;
      isLostConn = true;
      disconnectedState = true;
  }
}
}

void checkConnection(){
  if(strstr(outputBLE.c_str(), "OK+CONNA")){
    Serial.println(F("Conn"));
    bleOperationIndex = 0;
    oldStateBle = -1;
    masterState = false;
    connectedState = true;
  }
}
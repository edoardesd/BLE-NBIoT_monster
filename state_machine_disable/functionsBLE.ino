void checkOkBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      bleOperationIndex++;
    }
  }
}

// void checkDisconnBLE(){
//   if(!resetState){
//     if(strstr(outputBLE.c_str(), "OK+LOST")){
//       Serial.println(F("BLE DISCONNECTED, go with the next one"));
//       digitalWrite(LED_BUILTIN, LOW);
//       bleOperationIndex++;
//     }
//   }
// }

void checkResetBLE(){
  if(strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println(F("BLE set"));
    BLESerial.write("AT+NAME?");
    digitalWrite(LED_BUILTIN, LOW);


  if(strstr(stateMachine.ActiveStateName(), "S_BLE")){
    bleOperationIndex = 0;
    oldStateBle = -1;
    setupBLEState = false;
    setupNBIOTState = true;
  }

  //connected
    //  if(strstr(stateMachine.ActiveStateName(), "BLE_C")){
    // bleOperationIndex = 0;
    // oldStateBle = -1;
    // isconnectedState = false;
    // sleepState = true;
    // }
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
      macFound = true;
    }
  }

  if(strstr(outputBLE.c_str(), "OK+DISCE")){
    if(macFound){
      char connectCMD[13];
      strcat(connectCMD, "AT+CON");
      strcat(connectCMD, MAC_TO_CONNECT);
      Serial.println(connectCMD);
      BLESerial.write(connectCMD);
    }else{
      Serial.println(F("WARN no Mac "));
      masterState = false;
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
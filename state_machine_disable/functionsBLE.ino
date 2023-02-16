void checkOkBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+Set")){
      bleOperationIndex++;
    }
  }
}

void checkName(){
  if(strstr(outputBLE.c_str(), "OK+Set:m")){
    BLESerial.write("AT+RESET");    
  }
}

void checkDisconnBLE(){
  if(!resetState){
    if(strstr(outputBLE.c_str(), "OK+LOST") || strstr(outputBLE.c_str(), "OK")){
      // Serial.println(F("BLE DISC"));
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
      Serial.println(F("DisconDone"));

    }
  }
}

void checkMessageBLE(){
  if(strstr(outputBLE.c_str(), "6d")){
    Serial.println(F("Rqst from node"));
    strcpy(forwardPayload, outputBLE.c_str());
    forwardPayload[1] = '6';
    Serial.println(forwardPayload);
  }
}

void getConnectString(char *mac){
  strcat(connectCMD, CONNECT_TAG);
  strcat(connectCMD, mac);
  connectCMD[18] = '\0';
}

bool electMAC(){
  uint8_t device = selectDevice();
  if (device < 9){
    // strcpy(designatedMAC, devices_record[device].dev_mac);
    dev_index = 0;
    Serial.println(F("MAC fnd"));
    getConnectString(devices_record[device].dev_mac);

    return true;
  } 
  return false;
}

void checkDiscovery(){
  if(strstr(outputBLE.c_str(), "OK+DISCE")){
    readStruct();
    // electMAC();
    
    if(electMAC()){
      Serial.println(connectCMD);
      BLESerial.write(connectCMD);
      connectCMD[0] = '\0';
      memset(connectCMD, 0, sizeof connectCMD);
      // Serial.print(F("connect cmd: "));
      // Serial.println(connectCMD);

      // mac_found = false;
    }else{
      Serial.println(F("WARN no Mac "));
      masterState = false;
      isLostConn = true;
      disconnectedState = true;
  }
  }

  if(read_next){
    char name[10] = "";
    getName((char*)outputBLE.c_str(), name);
    split(name, id, rssi, transmissions);
    Serial.print(mac);
    Serial.print(" ");    
    Serial.print(id);
    Serial.print(" ");
    Serial.print(rssi);
    Serial.print(" ");
    Serial.println(transmissions);
    storeInStruct(mac, id, atoi(rssi), atoi(transmissions));
  } 
  else {
    read_next = getMac((char*)outputBLE.c_str(), mac);
  }
  // if(strstr(outputBLE.c_str(), "OK+DISC") || std::strstr(line, "OK+DIS1:")){
  //   char mac[13];
  //   char *mac_start = strchr(outputBLE.c_str(), ':');
  //   uint8_t index = (uint8_t)(mac_start - outputBLE.c_str()+1);
  //   strncpy(mac, &outputBLE.c_str()[index], 12);
  //   mac[12]='\0';

  //   if(strstr(mac, MAC_TO_CONNECT)){
  //     Serial.println(F("MAC fnd"));
  //     mac_found = true;
  //   }
  // }

}

void checkConnection(){
  if(strstr(outputBLE.c_str(), "OK+CONNA")){
    Serial.println(F("Con"));
    bleOperationIndex = 0;
    oldStateBle = -1;
    masterState = false;
    connectedState = true;
  }
}
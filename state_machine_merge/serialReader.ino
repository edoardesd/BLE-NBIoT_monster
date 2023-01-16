

void readBLE(){
  if (BLESerial.available()) {
    Serial.print(F("HM10: "));
    
    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    if(strstr(stateMachine.ActiveStateName(), "S_BLE") || 
      strstr(stateMachine.ActiveStateName(), "BLE_M")){
      checkOkBLE(); // check only if in right state...
      checkResetBLE();
    }

    if(strstr(stateMachine.ActiveStateName(), "BLE_M")){
      checkDiscovery();
      checkConnection();
    }

    if(!resetState || !setupBLEState || !setupNBIOTState){
      // check message and react
      checkMessageBLE();
    }
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

    if(strstr(stateMachine.ActiveStateName(), "WAKE") || 
       strstr(stateMachine.ActiveStateName(), "NB_FWD")){
      checkRSSI();
      checkSendNBIOT();
    }
  }
}
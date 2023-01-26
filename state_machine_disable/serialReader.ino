

void readBLE(){
  if (BLESerial.available()) {
    Serial.print(F("HM10: "));
    
    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    if(strstr(stateMachine.ActiveStateName(), "BLE_S") || 
      strstr(stateMachine.ActiveStateName(), "BLE_M")  || 
      strstr(stateMachine.ActiveStateName(), "BLE_C")  || 
      strstr(stateMachine.ActiveStateName(), "BLE_DISC")){
      checkOkBLE();
      checkResetBLE();
    }

    if(strstr(stateMachine.ActiveStateName(), "BLE_M")){
      checkDiscovery();
      checkConnection();
    }

    if(strstr(stateMachine.ActiveStateName(), "BLE_DISC")){
      checkDisconnBLE();
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

    if(strstr(stateMachine.ActiveStateName(), "NB")){
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
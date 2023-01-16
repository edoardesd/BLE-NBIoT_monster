

void readBLE(){
  if (BLESerial.available()) {
    Serial.print(F("HM10: "));
    
    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    checkOkBLE();
    checkResetBLE();

    if(strstr(stateMachine.ActiveStateName(), "BLE_M")){
      checkDiscovery();
      checkConnection();
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

    if(strstr(stateMachine.ActiveStateName(), "WAKE")){
      checkRSSI();
      checkSendNBIOT();
    }
  }
}
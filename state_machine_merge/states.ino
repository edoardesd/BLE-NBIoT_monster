/////////// STATE MACHINE FUNCTIONS /////////////////
void resetInterfaces() {
  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.println(F("Rst intrfc"));
  Serial.println(F("RST NB"));
  NBIOTSerial.write("ATI9\r\n");
  NBIOTSerial.write("AT+NRB\r\n");
  delay(200);
  Serial.println(F("Rst BLE"));
  BLESerial.write("AT");
  delay(200);
  BLESerial.write("AT+RENEW");
}

void setupNBIoTConnection() {
  // if operation is increased
  if (nbIoToldState != okNBIOTList) {
    if (okNBIOTList < NUM_SETUPOPERATIONS_NBIOT) {
      cmdNBIOT = strcat(setupIoTList[okNBIOTList].c_str(), "\r\n");
      Serial.println(cmdNBIOT);
      NBIOTSerial.write(cmdNBIOT.c_str());
    }
    if (okNBIOTList == NUM_SETUPOPERATIONS_NBIOT + 1) {
      Serial.println(DGRAMcmd);
      NBIOTSerial.write(DGRAMcmd);
    }

    if (okNBIOTList == NUM_SETUPOPERATIONS_NBIOT + 2) {
      Serial.println(F("Sckt done"));
      digitalWrite(LED_BUILTIN, LOW);
      setupNBIOTState = false;
      sleepState = true;
    }
  }
  nbIoToldState = okNBIOTList;
}

void setupBlueToothConnection() {
  if (oldStateBle != bleOperationIndex) {
    if (bleOperationIndex < NUM_SETUPOPERATIONS_BLE) {  //Ordinary operations
      cmd_ble = setupBLEList[bleOperationIndex];
      Serial.println(cmd_ble);
      BLESerial.write(cmd_ble.c_str());
      oldStateBle = bleOperationIndex;
    }     
  }
}

void onWakeUp(){
    NBIOTSerial.write(STATScmd);
}

void sendNBIOT(){
  if(readyToSendNBIOT){
    NBIOTSerial.write(TRANScmd);
    readyToSendNBIOT = false;
  }
}

void onSleep(){
  sleepState = false;
}

void onMaster() {
  if (bleOperationIndex < NUM_MASTEROPERATIONS_BLE) {
    if (oldStateBle != bleOperationIndex) {  //master operations
      cmd_ble = masterBLEList[bleOperationIndex];
      Serial.println(cmd_ble);
      BLESerial.write(cmd_ble.c_str());
      oldStateBle = bleOperationIndex;
    }
  }
}

void bleConnected() {
  for (uint8_t i = 0; i < 10; i++) {
    Serial.print(F("Sending: "));
    Serial.println(messageBLE);
    BLESerial.write(messageBLE);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }
}


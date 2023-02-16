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
  // #ifdef NONBMODULE  
  //   Serial.println(F("NB stp skip")); 
  //   okNBIOTList = NUM_SETUPOPERATIONS_NBIOT + 2;
  // #endif

  if (nbIoToldState != okNBIOTList) {
    if (okNBIOTList < NUM_SETUPOPERATIONS_NBIOT) {
      Serial.println(setupIoTList[okNBIOTList]);
      NBIOTSerial.write(setupIoTList[okNBIOTList]);
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
      // if(strstr(setupBLEList[bleOperationIndex], "NAME")){
      //   createName(setupBLEList[bleOperationIndex]);
      // }
      Serial.println(setupBLEList[bleOperationIndex]);
      BLESerial.write(setupBLEList[bleOperationIndex]);
      oldStateBle = bleOperationIndex;
    }     
  }
}

void onWakeUp(){
  wakeState = true;
  Serial.println(F("READ RSRQ"));
  setupDatagram();
  // NBIOTSerial.write(STATScmd);
}

void sendNBIOT(){
  if(readyToSendNBIOT){
    // #ifndef NONBMODULE  
      Serial.println(TRANScmd);
      NBIOTSerial.write(TRANScmd);
    // #else
      // Serial.println(F("NB snd skpd"));
      // onSendNBIOT();
    // #endif
    readyToSendNBIOT = false;
  }
}

void forwardNBIOT(){
  delay(200);
  memset(payloadHex, 0, sizeof payloadHex);
  strlcpy(payloadHex, forwardPayload, sizeof(payloadHex));
  memset(forwardPayload, 0, sizeof forwardPayload);
  Serial.println(payloadHex);
  delay(200);
  createMessage();
  delay(200);
  Serial.println(F("Frward"));
  readyToSendNBIOT = true;
}

void onSleep(){
  uint8_t nameLen = 20;
  char newName[nameLen];
  strlcpy(newName, "AT+NAME", sizeof(newName));
  createName(newName, nameLen);
  BLESerial.write(newName);
  isLostConn = false;
  sleepState = false;
  forwardState = false;
}

void onMaster() {
  if (bleOperationIndex < NUM_MASTEROPERATIONS_BLE) {
    if (oldStateBle != bleOperationIndex) {  //master operations
      Serial.println(masterBLEList[bleOperationIndex]);
      BLESerial.write(masterBLEList[bleOperationIndex]);
      oldStateBle = bleOperationIndex;
    }
  }
}

void bleConnected() {

  Serial.print(F("Snd: "));
  Serial.println(payloadHex);
  BLESerial.write(payloadHex);
  for (i = 0; i < 5; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

}

void bleDiscLoop(){
  if(isLostConn){
    if (bleOperationIndex < NUM_DISCONNOPERATIONS_BLE) {
      if (oldStateBle != bleOperationIndex) { 
        // cmd_ble = masterBLEList[bleOperationIndex];
        Serial.println(disconnectionBLEList[bleOperationIndex]);
        BLESerial.write(disconnectionBLEList[bleOperationIndex]);
        oldStateBle = bleOperationIndex;
      }
    }
  }
}

void bleDisc(){
  bleOperationIndex = 0;
  oldStateBle = -1;
  dev_index = 0;
  connectedState = false;
  // disconnectedState = false;
  Serial.println(F("Disc"));
  BLESerial.write("AT");
  delay(200); //go to disconnect loop, no sleep direct
  // sleepState = true;
}


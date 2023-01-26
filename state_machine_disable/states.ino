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
  #ifdef NONBMODULE  
    Serial.println(F("NB stp skip")); 
    okNBIOTList = NUM_SETUPOPERATIONS_NBIOT + 2;
  #endif

  if (nbIoToldState != okNBIOTList) {
    if (okNBIOTList < NUM_SETUPOPERATIONS_NBIOT) {
      cmdNBIOT = strcat(setupIoTList[okNBIOTList], "\r\n");
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
      if(strstr(cmd_ble.c_str(), "NAME")){
        strcat(cmd_ble.c_str(), BLENAME);
        strcat(cmd_ble.c_str(), "-121");
      }
      Serial.println(cmd_ble);
      BLESerial.write(cmd_ble.c_str());
      oldStateBle = bleOperationIndex;
    }     
  }
}

void onWakeUp(){
  Serial.println(SLEEP_TIME);
  NBIOTSerial.write(STATScmd);
}

void sendNBIOT(){
  if(readyToSendNBIOT){
    #ifndef NONBMODULE  
      NBIOTSerial.write(TRANScmd);
    #else
      Serial.println(F("NB snd skpd"));
      onSendNBIOT();
    #endif
    readyToSendNBIOT = false;
  }

  
}

void forwardNBIOT(){
  strcpy(payloadHex, forwardPayload);
  memset(forwardPayload, 0, sizeof forwardPayload);
  Serial.println(payloadHex);
  createMessage();
  Serial.println(F("Frwardng"));
  readyToSendNBIOT = true;
}

void onSleep(){
  sleepState = false;
  forwardState = false;
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
  Serial.print(F("Snd: "));
  Serial.println(payloadHex);
  BLESerial.write(payloadHex);
  for (i = 0; i < 5; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  // memset(payloadHex, 0, sizeof payloadHex);

  
}

void bleDiscLoop(){
  if(isLostConn){
    if (bleOperationIndex < NUM_DISCONNOPERATIONS_BLE) {
      if (oldStateBle != bleOperationIndex) { 
        cmd_ble = disconnectionBLEList[bleOperationIndex];
        Serial.println(cmd_ble);
        BLESerial.write(cmd_ble.c_str());
        oldStateBle = bleOperationIndex;
      }
    }
  }
}

void bleDisc(){
  bleOperationIndex = 0;
  oldStateBle = -1;
  connectedState = false;
  // disconnectedState = false;
  Serial.println(F("Disc"));
  BLESerial.write("AT");
  delay(200); //go to disconnect loop, no sleep direct
  // sleepState = true;
}


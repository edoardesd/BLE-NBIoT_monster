/////////// STATE MACHINE FUNCTIONS /////////////////
void resetInterfaces() {
  Serial.println("Resetting the interfaces");
  delay(200);
  Serial.println("Reset BLE");
  Serial.println("AT");
  BLESerial.write("AT");
  delay(200);
  Serial.println("AT+RENEW");
  BLESerial.write("AT+RENEW");
  delay(200);

  resetState = false;
  setupBLEState = true;
}

void bleAdvertisement() {
  // reset states for master list oprations
  oldStateBle = -1;
  bleOperationIndex = 0;

  if(isReceived){
    if(millis() > time_now + period){
      Serial.println("reset timer");
      isReceived = false;
    }
  }
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
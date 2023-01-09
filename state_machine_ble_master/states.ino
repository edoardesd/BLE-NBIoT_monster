/////////// STATE MACHINE FUNCTIONS /////////////////
void resetInterfaces() {
  Serial.println("Resetting the interfaces");
  // Serial.println("Reset NB-IoT");
  // NBIOTSerial.write("ATI9\r\n");
  // NBIOTSerial.write("AT+NRB\r\n");
  delay(200);
  Serial.println("Reset BLE");
  Serial.println("AT");
  BLESerial.write("AT");
  delay(200);
  Serial.println("AT+RENEW");
  BLESerial.write("AT+RENEW");
  delay(200);

  digitalWrite(LED_BUILTIN, HIGH);

  resetState = false;
  setupBLEState = true;
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
  for (int i = 0; i < 10; i++) {
    Serial.print("Sending: ");
    Serial.println(message);
    digitalWrite(LED_BUILTIN, HIGH);
    // BLESerial.write(message);
    delay(100);
  }

  bleTransmissions++;
}

void inTransmission() {
  if (bleTransmissions > NUM_BLE_MESSAGES - 1) {
    bleConnectedState = false;
    bleDisconnectionState = true;
  }
}

void bleDisconnection() {
  if (oldStateBle != bleOperationIndex) {
    if (bleOperationIndex < NUM_DISCONNOPERATIONS_BLE) {  //Ordinary operations
      cmd_ble = disconnectionBLEList[bleOperationIndex];
      Serial.println(cmd_ble);
      BLESerial.write(cmd_ble.c_str());
      oldStateBle = bleOperationIndex;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      bleTransmissions = 0;
      bleOperationIndex = 0;
      oldStateBle = -1;
      bleDisconnectionState = false;
      bleAdvState = true;
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


// Serial.println("AT+ROLE0");
// BLESerial.write("AT+ROLE0");  // set the role as slave
// delay(200);
// Serial.println("AT+IMME0");
// BLESerial.write("AT+IMME0");  // 1 = Only on. Until AT + START, AT+CON, AT+CONNL commands is received. // 0: on and active to work -> IMME1 when needs to become central mode
// delay(200);

// Serial.println(strcat("AT+NAME", BLENAME));
// BLESerial.write(strcat("AT+NAME", BLENAME));
// //BLESerial.write(strcat("AT+PASS", 12345));

// delay(200);
// Serial.println("AT+AUTH1");
// BLESerial.write("AT+AUTH1");  //1 auto , 0 not auto?
// delay(200);
// Serial.println("AT+NOTI1");
// BLESerial.write("AT+NOTI1");  // Enable notifications (e.g. conn/disconn)
// delay(200);
// Serial.println("AT+NOTP1");
// BLESerial.write("AT+NOTP1");  // Show also the address of the notification
// delay(200);
// Serial.println("AT+ADTY0");
// BLESerial.write("AT+ADTY0");  // 0 connectable advertising scanresponse // 1 and 2 not useful for us // 3 only advertising
// delay(200);
// Serial.println("AT+BAUD0");
// BLESerial.write("AT+BAUD0");  // 0 = 9600 baudAT+CHAR?
// delay(200);
// Serial.println("AT+ADVI1");
// BLESerial.write("AT+ADVI1");  // speed of the transmission of the advertisements, 0 = 100ms F= 9000 ms
// //BLSSerial.write("AT+FLAG0"); // allow advertisements, should work even without the command
// delay(200);
// Serial.println("AT+SHOW3");
// BLESerial.write("AT+SHOW3");  // in discovery: 1 only name, 2 only rssi, 3 both (3 is too much sometimes)
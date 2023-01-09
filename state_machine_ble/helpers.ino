void readSerial() {
  if (Serial.available()) {
    str = "";
    Serial.print("Input: ");

    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c != 10 && c != 13) {  // Don't send line end characters to HM10.
          str += c;
        }
      }
    }

    if (strstr(str.c_str(), BLE_TAG)) {
      Serial.print("BLE Stream: ");
      str = strremove(str.c_str(), BLE_TAG);
      BLESerial.print(str);
    }

    if (strstr(str.c_str(), NBIOT_TAG)) {
      Serial.print("NB-IoT Stream: ");
      str = strremove(str.c_str(), NBIOT_TAG);
      str = strcat(str.c_str(), "\r\n");
      NBIOTSerial.write(str.c_str());
    }
    Serial.println(str);
  }
}

char *strremove(char *str, const char *sub) {
  size_t len = strlen(sub);
  if (len > 0) {
    char *p = str;
    while ((p = strstr(p, sub)) != NULL) {
      memmove(p, p + len, strlen(p + len) + 1);
    }
  }
  return str;
}

void readNBIOT() {
  if (NBIOTSerial.available()) {
    outputNBIOT = NBIOTSerial.readStringUntil('\n');
    Serial.println(outputNBIOT);
  }
}


void checkOkBLE() {
  if (!resetState) {
    if (strstr(outputBLE.c_str(), "OK+Set")) {
      Serial.println("Operation ble done, go with the next one");
      bleOperationIndex++;
    }
  }
}

void checkDisconnBLE() {
  if (!resetState) {
    if (strstr(outputBLE.c_str(), "OK+LOST")) {
      Serial.println("BLE DISCONNECTED, go with the next one");
      bleOperationIndex++;
    }
  }
}

void checkResetBLE() {
  if (strstr(outputBLE.c_str(), "OK+RENEW")) {
    Serial.println("Hard reset done");
    resetState = false;
    setupBLEState = true;
  }

  if (strstr(outputBLE.c_str(), "OK+RESET")) {
    Serial.println("BLE setup done.");
    // BLESerial.write("AT+NAME?");

    if (strstr(stateMachine.ActiveStateName(), "SETUP_BLE")) {
      digitalWrite(LED_BUILTIN, LOW);
      bleOperationIndex = 0;
      BLESerial.write("AT+SLEEP");
      isWakeUp = false;
      oldStateBle = -1;
      setupBLEState = false;
      bleAdvState = true;
    }

    if (strstr(stateMachine.ActiveStateName(), "BLE_DISCONNECTION")) {
      bleOperationIndex = 0;
      oldStateBle = -1;
      bleDisconnectionState = false;
      bleAdvState = true;
    }
  }
}

void checkSleep(){
  // TODO  
}

void checkWakeUpBLE() {
  if (!resetState) {
    if (strstr(outputBLE.c_str(), "OK+WAKE")) {
      Serial.println("Wake up done");
      isWakeUp = true;
    }
  }
}

void checkDiscovery() {
  char mac[13];
  char *mac_start;
  int index;

  if (strstr(outputBLE.c_str(), "OK+DIS")) {
    mac_start = strchr(outputBLE.c_str(), ':');
    index = (int)(mac_start - outputBLE.c_str() + 1);
    strncpy(mac, &outputBLE.c_str()[index], 12);
    mac[12] = '\0';
    Serial.println(mac);

    if (strstr(mac, MAC_TO_CONNECT)) {
      Serial.println("MAC ADDRESS FOUND, start the connect!");
      // BLESerial.write(strcat("AT+CON", MAC_TO_CONNECT);
    }
  }

  if (strstr(outputBLE.c_str(), "OK+DISCE")) {
    BLESerial.write(strcat("AT+CON", MAC_TO_CONNECT));
  }
}

void checkConnection() {
  if (strstr(outputBLE.c_str(), "OK+CONNA")) {
    Serial.println("Connected!");
    bleOperationIndex = 0;
    oldStateBle = -1;
    bleMasterState = false;
    bleConnectedState = true;
  }
}
void readBLE() {
  if (BLESerial.available()) {
    Serial.print("HM10: ");

    outputBLE = BLESerial.readStringUntil('\n');
    Serial.println(outputBLE);
    checkOkBLE();
    checkResetBLE();
    checkWakeUpBLE();
    checkSleep();

    if (strstr(stateMachine.ActiveStateName(), "BLE_DISCONNECTION")) {

      checkDisconnBLE();
    }

    if (strstr(stateMachine.ActiveStateName(), "BLE_MASTER")) {
      checkDiscovery();
      checkConnection();
    }
  }
}
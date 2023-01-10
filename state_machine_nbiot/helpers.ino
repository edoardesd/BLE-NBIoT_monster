void readSerial() {
  if (Serial.available()) {
    String str = "";
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

void checkReset() {
  if (strstr(outputNBIOT.c_str(), RESET_NBIOT_TAG)) {
    Serial.println("NBIoT Reset done.");
    resetState = false;
    setupNBIOTState = true;
  }
}

void checkOk() {
  if (!resetState) {
    if (strstr(outputNBIOT.c_str(), SUCCESS_NBIOT_TAG)) {
      // Serial.println("Operation done, go with the next one");
      okNBIOTList++;
      if (statsState) {
        statsState = false;
        transState = true;
      }
    }
  }
}

void checkConnection() {
  if (setupNBIOTState) {
    if (strstr(outputNBIOT.c_str(), CEREG_NBIOT_TAG)) {
      Serial.println("Connected!");
      delay(200);
      NBIOTSerial.write(CGATTcmd.c_str());
    }
  }
}

void createMessage() {
  if (idDatagram < 10) {
    stringIdDatagram = "00" + String(idDatagram);
  }
  if (idDatagram >= 10 && idDatagram < 100) {
    stringIdDatagram = "0" + String(idDatagram);
  }
  if (idDatagram >= 100 && idDatagram < 1000) {
    stringIdDatagram = String(idDatagram);
  }

  strcat(payload, stringIdDatagram.c_str());
  Serial.println(payload);

  strcat(payload, "A");
  Serial.println(payload);

  strcat(payload, rsrq);



  Serial.println(payload);

  strcat(payload, remainingPayload);
  strcat(payload, remainingPayload);

  Serial.println(payload);

  int len = strlen(payload);

  for (int i = 0, j = 0; i < len; ++i, j += 2) {
    sprintf(payloadHex + j, "%02x", payload[i] & 0xff);
  }

  Serial.println(payloadHex);

  strcat(TRANScmd, "AT+NSOST=0,\"131.175.120.22\",8883,");
  snprintf(buffer, sizeof(buffer), "%d", len);
  strcat(TRANScmd, buffer);
  strcat(TRANScmd, ",\"");
  strcat(TRANScmd, payloadHex);
  strcat(TRANScmd, "\"\r\n");
}

void checkRSSI() {
  if (!resetState) {
    if (strstr(outputNBIOT.c_str(), "RSRQ")) {
      strcpy(rsrq, strremove(strremove(outputNBIOT.c_str(), "\"RSRQ\",-"), "\r"));
      // int rsrqInt = rsrq.toInt();

      memset(payload, 0, sizeof payload);
      memset(TRANScmd, 0, sizeof TRANScmd);

      createMessage();

        Serial.println();
      Serial.println(TRANScmd);
    }
  }
}

void readNBIOT() {
  if (NBIOTSerial.available()) {
    outputNBIOT = NBIOTSerial.readStringUntil('\n');
    Serial.println(outputNBIOT);
    checkReset();
    checkConnection();
    checkOk();
    checkRSSI();
  }
}
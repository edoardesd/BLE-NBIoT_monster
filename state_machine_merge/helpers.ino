void readSerial(){
  if (Serial.available()) {
    String str = "";
    Serial.print(F("Input: "));
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c != 10 && c != 13) { // Don't send line end characters to HM10.
          str += c;
        }
      }
    }

    if (strstr(str.c_str(), FORCE_TAG)){
      Serial.println(F("WARNING: NB-IoT not available"));
      forceBLE = true;
    }

    if (strstr(str.c_str(), BLE_TAG)){
      Serial.print(F("BLE Stream: "));
      str = strremove(str.c_str(), BLE_TAG);
      BLESerial.print(str);
    }

    if (strstr(str.c_str(), NBIOT_TAG)){
      Serial.print(F("NB-IoT Stream: "));
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

void createIdDatagram(){
  if (idDatagram < 10) {
    stringIdDatagram = "00" + String(idDatagram);
  }
  if (idDatagram >= 10 && idDatagram < 100) {
    stringIdDatagram = "0" + String(idDatagram);
  }
  if (idDatagram >= 100 && idDatagram < 1000) {
    stringIdDatagram = String(idDatagram);
  }
  if (idDatagram >= 1000){
    stringIdDatagram = 999;
    idDatagram = 0;
  }
  idDatagram++;
}
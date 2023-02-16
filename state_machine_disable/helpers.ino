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
      Serial.println(F("WARN:no NBIoT"));
      forceNBIOT = true;
    }

    if (strstr(str.c_str(), RELEASE_TAG)){
       Serial.println(F("WARN:actv NBIoT"));
      forceNBIOT = false;
    }

    if (strstr(str.c_str(), BLE_TAG)){
      Serial.print(F("BLE: "));
      str = strremove(str.c_str(), BLE_TAG);
      BLESerial.print(str);
    }

    if (strstr(str.c_str(), NBIOT_TAG)){
      Serial.print(F("NBIoT: "));
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

String createIdDatagram(){
  String stringIdDatagram;
  char strId[4];

  if (idDatagram < 10) {
    sprintf(strId,"00%d",idDatagram);
    strcat(stringIdDatagram.c_str(), strId);
  }
  if (idDatagram >= 10 && idDatagram < 100) {
    sprintf(strId,"0%d",idDatagram);
    strcat(stringIdDatagram.c_str(), strId);
  }
  if (idDatagram >= 100 && idDatagram < 1000) {
    strcat(stringIdDatagram.c_str(), strId);
  }
  if (idDatagram >= 1000){
    strcpy(stringIdDatagram.c_str(), "999");
    idDatagram = 0;
  }
  idDatagram++;
  return stringIdDatagram;
}

void createName(char *name){
  char strTrans[4];
  strcat(name, BLENAME);
  strcat(name, "-");
  strcat(name, current_rsrq);
  strcat(name, "-");
  sprintf(strTrans,"%d",totalTransmissions);
  strcat(name, strTrans);
  name[19] = '\0'; 
  Serial.println(name);
}
